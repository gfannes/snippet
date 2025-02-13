const std = @import("std");
const ut = std.testing;

const Strange = struct {
    content: []const u8,

    const StrCh = struct {
        str: []const u8,
        ch: u8,
    };

    fn pop_to(self: *Strange, chars: []const u8) ?StrCh {
        var ix: usize = 0;
        while (ix < self.content.len) : (ix += 1) {
            const ch = self.content[ix];
            if (ch == '\\') {
                // We accept whatever character comes after the escape.
                // If self.content would end on an incomplete escape and no match was found yet, we will return null.
                ix += 1;
            } else {
                if (contains(chars, ch)) {
                    const ret = StrCh{ .str = self.content[0..ix], .ch = ch };
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

    fn pop_ch(self: *Strange, ch: u8) bool {
        if (self.content.len == 0 or self.content[0] != ch) return false;
        self.content.ptr += 1;
        self.content.len -= 1;
        return true;
    }

    test "Strange.pop_ch" {
        var strange: Strange = undefined;
        {
            strange = Strange{ .content = "abc" };
            try ut.expect(strange.pop_ch('a'));
            try ut.expect(!strange.pop_ch('a'));
            try ut.expect(strange.pop_ch('b'));
            try ut.expect(strange.pop_ch('c'));
            try ut.expect(!strange.pop_ch('c'));
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
};

const State = enum { Body, Tag, Attr };

const Item = enum { Text, Open, Attr, Close };

fn parse_node(strange: *Strange, cb: anytype) !void {
    if (strange.pop_to("]")) |tag| {
        cb.call(Item.Open, tag.str);
        while (strange.pop_to("([{")) |strch| {
            switch (strch.ch) {
                '(' => if (strange.pop_to(")")) |attr| {
                    cb.call(Item.Attr, attr.str);
                },
                '[' => {
                    cb.call(Item.Close, strch.str);
                    try parse_node(strange, cb);
                },
                '{' => if (strange.pop_to("[}")) |strch2| {
                    switch (strch2.ch) {
                        '[' => {
                            cb.call(Item.Text, strch2.str);
                            try parse_node(strange, cb);
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

fn parse(content: []const u8, cb: anytype) !void {
    var strange = Strange{ .content = content };
    if (strange.pop_to("[")) |text| {
        cb.call(Item.Text, text.str);
        try parse_node(&strange, cb);
    }
}

test "ut" {
    const cb = struct {
        pub fn call(_: @This(), item: Item, str: []const u8) void {
            std.debug.print("[{}](str:{s})\n", .{ item, str });
        }
    }{};
    try parse("abc[tag](attr1)(attr2){text}", cb);
}
