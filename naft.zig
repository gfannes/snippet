const std = @import("std");

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
    fn pop_ch(self: *Strange, ch: u8) bool {
        if (self.content.len == 0) return false;
        const ret = self.content[0] == ch;
        self.content.ptr += 1;
        self.content.len -= 1;
        return ret;
    }

    fn contains(chars: []const u8, ch: u8) bool {
        for (chars) |c| {
            if (c == ch) return true;
        }
        return false;
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

pub fn main() !void {
    const cb = struct {
        pub fn call(_: @This(), item: Item, str: []const u8) void {
            std.debug.print("[{}](str:{s})\n", .{ item, str });
        }
    }{};
    try parse("abc[tag](attr1)(attr2){text}", cb);
}
