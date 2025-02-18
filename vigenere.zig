const std = @import("std");
const ut = std.testing;

const Coder = struct {
    code: []const u8,
    ix: usize = 0,

    pub fn init(code: []const u8) Coder {
        return Coder{ .code = code };
    }

    pub fn encode_ch(self: *Coder, ch: u8) u8 {
        const shift = self.code[self.ix] - 'a';
        std.debug.print("shift: {}\n", .{shift});
        self.ix = (self.ix + 1) % self.code.len;
        return 'a' + (ch - 'a' + shift) % 26;
    }

    pub fn encode_str(self: *Coder, str: []u8) void {
        for (str) |*ch| {
            ch.* = self.encode_ch(ch.*);
        }
    }
};

test "Coder.encode_ch" {
    var coder = Coder.init("abc");
    try ut.expectEqual('a', coder.encode_ch('a'));
    try ut.expectEqual('b', coder.encode_ch('a'));
    try ut.expectEqual('c', coder.encode_ch('a'));
    try ut.expectEqual('a', coder.encode_ch('a'));
}
test "Coder.encode_str" {
    var coder = Coder.init("abc");
    var msg = [_]u8{ 'a', 'a', 'a' };
    coder.encode_str(&msg);
    try ut.expectEqualSlices(u8, "abc", &msg);
}

// Counts self-matches when shifting enc with a certain amount.
pub fn analyse(comptime n: usize, enc: []const u8) [n]usize {
    var ret: [n]usize = undefined;
    for (&ret, 0..) |*count, shift| {
        count.* = 0;
        for (enc, shift..) |ch, ix| {
            const ch_shift = enc[ix % enc.len];
            count.* += if (ch == ch_shift) 1 else 0;
        }
    }
    return ret;
}
test "analyse" {
    try ut.expectEqual([_]usize{ 4, 1, 0, 1 }, analyse(4, "abbc"));
    try ut.expectEqual([_]usize{ 15, 3, 0, 0, 3, 15 }, analyse(6, "geertgeertgeert"));
}
