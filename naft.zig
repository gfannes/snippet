const std = @import("std");
const ut = std.testing;

const NaftError = error{
    UnclosedTag,
    UnclosedAttr,
    BlockNest,
};

const Strange = struct {
    content: []const u8,

    const StrCh = struct {
        str: []const u8,
        ch: u8,
        escape: bool = false,
    };

    fn pop_to(self: *Strange, chars: []const u8) ?StrCh {
        var ix: usize = 0;
        var escape = false;
        while (ix < self.content.len) : (ix += 1) {
            const ch = self.content[ix];
            if (ch == '\\') {
                // We accept whatever character comes after the escape.
                // If self.content would end on an incomplete escape and no match was found yet, we will return null.
                ix += 1;
                escape = true;
            } else {
                if (contains(chars, ch)) {
                    const ret = StrCh{ .str = self.content[0..ix], .ch = ch, .escape = escape };
                    self.content.ptr += ix + 1;
                    self.content.len -= ix + 1;
                    return ret;
                }
            }
        }
        return null;
    }

    test "Strange.pop_to" {
        var strange: Strange = undefined;
        {
            strange = Strange{ .content = "abc" };
            if (strange.pop_to("a")) |strch| {
                try ut.expect(std.mem.eql(u8, strch.str, ""));
                try ut.expect(strch.ch == 'a');
            } else unreachable;
        }
        {
            strange = Strange{ .content = "abc" };
            if (strange.pop_to("cb")) |strch| {
                try ut.expect(std.mem.eql(u8, strch.str, "a"));
                try ut.expect(strch.ch == 'b');
            } else unreachable;
        }
        {
            strange = Strange{ .content = "abc" };
            if (strange.pop_to("d")) |_| {
                unreachable;
            }
        }
    }

    fn contains(chars: []const u8, ch: u8) bool {
        for (chars) |c| {
            if (c == ch) return true;
        }
        return false;
    }

    test "Strange.contains" {
        try ut.expect(Strange.contains("abc", 'a'));
        try ut.expect(Strange.contains("abc", 'a'));
        try ut.expect(Strange.contains("abc", 'b'));
        try ut.expect(Strange.contains("abc", 'c'));
        try ut.expect(!Strange.contains("abc", 'd'));
        try ut.expect(!Strange.contains("", 'a'));
    }

    fn pop_all(self: *Strange) ?StrCh {
        if (self.content.len == 0) return null;
        const ret = StrCh{ .str = self.content, .ch = 0, .escape = contains(self.content, '\\') };
        self.content.ptr += self.content.len;
        self.content.len = 0;
        return ret;
    }

    test "Strange.pop_all" {
        const Scn = struct {
            content: []const u8,
            escape: bool = false,
        };
        const scns = [_]Scn{ .{ .content = "abc" }, .{ .content = "a\\c", .escape = true } };
        for (scns) |scn| {
            var strange = Strange{ .content = scn.content };
            if (strange.pop_all()) |strch| {
                try ut.expectEqualSlices(u8, scn.content, strch.str);
                try ut.expect(strch.ch == 0);
                try ut.expect(strch.escape == scn.escape);
            } else unreachable;
            try ut.expect(strange.content.len == 0);
        }
    }
};

const Parser = struct {
    strange: Strange,
    level: u32 = 0,

    fn parse_node(self: *Parser, cb: anytype) !void {
        if (self.strange.pop_to("]")) |tag| {
            cb.call(Item.Tag, tag.str);
            while (self.strange.pop_to("([{")) |strch| {
                switch (strch.ch) {
                    '(' => if (self.strange.pop_to(")")) |attr| {
                        cb.call(Item.Attr, attr.str);
                    },
                    '[' => {
                        cb.call(Item.Close, strch.str);
                        try self.parse_node(cb);
                    },
                    '{' => if (self.strange.pop_to("[}")) |strch2| {
                        switch (strch2.ch) {
                            '[' => {
                                cb.call(Item.Text, strch2.str);
                                try self.parse_node(cb);
                            },
                            '}' => cb.call(Item.Close, strch.str),
                            else => {},
                        }
                    },
                    else => {},
                }
            }
        }
    }

    fn parse_block(self: *Parser, cb: anytype) !void {
        var text_opt = self.strange.pop_to("[");
        while (text_opt) |text| {
            if (text.str.len > 0)
                try cb.call(Item.Text, text.str, text.escape);
            if (self.strange.pop_to("]")) |tag| {
                try cb.call(Item.Tag, tag.str, tag.escape);
                while (true) {
                    const needle = if (self.level == 0) "([{" else "([{}";
                    const strch_opt = self.strange.pop_to(needle);
                    if (strch_opt) |strch| {
                        switch (strch.ch) {
                            '(' => {
                                if (strch.str.len > 0)
                                    try cb.call(Item.Text, strch.str, strch.escape);
                                if (self.strange.pop_to(")")) |attr| {
                                    try cb.call(Item.Attr, attr.str, attr.escape);
                                } else return NaftError.UnclosedAttr;
                            },
                            '[' => {
                                // We found another Tag at the same level: continue the outer while loop
                                text_opt = strch_opt;
                                break;
                            },
                            '{' => {
                                try cb.call(Item.Block, strch.str, strch.escape);
                                self.level += 1;
                                try self.parse_block(cb);
                            },
                            '}' => {
                                if (self.level == 0)
                                    return NaftError.BlockNest;
                                self.level -= 1;
                                try cb.call(Item.Close, strch.str, strch.escape);
                            },
                            else => unreachable,
                        }
                    } else {
                        text_opt = null;
                        break;
                    }
                }
            } else return NaftError.UnclosedTag;
        }

        if (self.strange.pop_all()) |strch| {
            try cb.call(Item.Text, strch.str, strch.escape);
        }
    }
};

test "Parser.parse_block" {
    const testit = struct {
        fn call(content: []const u8, exp: []const u8) !void {
            const strange = Strange{ .content = content };
            var parser = Parser{ .strange = strange };

            var gpa = std.heap.GeneralPurposeAllocator(.{}){};
            var aa = std.heap.ArenaAllocator.init(gpa.allocator());
            defer aa.deinit();
            const a = aa.allocator();

            const Parts = std.ArrayList([]const u8);
            var parts = Parts{};

            var cb = struct {
                a: std.mem.Allocator,
                parts: *Parts,

                pub fn call(self: *@This(), item: Item, str: []const u8, escape: bool) !void {
                    std.debug.print("CB: {} {s} {}\n", .{ item, str, escape });
                    const item_str = switch (item) {
                        Item.Text => "Text",
                        Item.Tag => "Tag",
                        Item.Attr => "Attr",
                        Item.Block => "Block",
                        Item.Close => "Close",
                    };
                    try self.parts.append(self.a, try std.fmt.allocPrint(self.a, "({s}:{s})", .{ item_str, str }));
                }
            }{ .a = a, .parts = &parts };
            try parser.parse_block(&cb);

            const act = try std.mem.concat(a, u8, parts.items);

            try ut.expectEqualSlices(u8, exp, act);
        }
    }.call;

    try testit("abc", "(Text:abc)");
    try testit("[tag]", "(Tag:tag)");
    try testit("[tag](attr1)", "(Tag:tag)(Attr:attr1)");
    try testit("[tag](attr1)(attr2)", "(Tag:tag)(Attr:attr1)(Attr:attr2)");
    try testit("[tag1][tag2]", "(Tag:tag1)(Tag:tag2)");
    try testit("[a]{[b][c]}", "(Tag:a)(Block:)(Tag:b)(Tag:c)(Close:)");
}

const State = enum { Body, Tag, Attr };

const Item = enum { Text, Tag, Attr, Block, Close };

fn parse(content: []const u8, cb: anytype) !void {
    var parser = Parser{ .strange = Strange{ .content = content } };
    if (parser.strange.pop_to("[")) |text| {
        cb.call(Item.Text, text.str);
        try parser.parse_node(cb);
    }
}

test "parse" {
    const cb = struct {
        pub fn call(_: @This(), item: Item, str: []const u8) void {
            std.debug.print("[{}](str:{s})\n", .{ item, str });
        }
    }{};
    try parse("abc[tag](attr1)(attr2){text}", cb);
}
