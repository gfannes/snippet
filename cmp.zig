const std = @import("std");

const File = struct {
    const Self = @This();

    buffer: [1024 * 1024]u8 = undefined,

    file: std.fs.File = undefined,
    stat: std.fs.File.Stat = undefined,
    rbuf: [1024]u8 = undefined,
    reader: std.fs.File.Reader = undefined,
    io: *std.Io.Reader = undefined,

    fn init(self: *Self, path: []const u8) !void {
        self.file = try std.fs.openFileAbsolute(path, .{});
        self.stat = try self.file.stat();
        self.reader = self.file.reader(&self.rbuf);
        self.io = &self.reader.interface;
    }
    fn deinit(self: *Self) void {
        self.file.close();
    }
};

test "cmp" {
    const ut = std.testing;

    var fa = File{};
    try fa.init("/home/geertf/aws/auro-masv-transfer/AURO TEST DISC/AURO_TESTDISC_V10.iso");
    defer fa.deinit();

    var fb = File{};
    try fb.init("/home/geertf/story/cd13/AURO_TESTDISC_V10.iso");
    defer fb.deinit();

    try ut.expectEqual(fa.stat.size, fb.stat.size);

    var ix: usize = 0;
    while (ix < fa.stat.size) {
        const size = @min(fa.buffer.len, fa.stat.size - ix);

        const sa = fa.buffer[0..size];
        try fa.io.readSliceAll(sa);

        const sb = fb.buffer[0..size];
        try fb.io.readSliceAll(sb);

        if (!std.mem.eql(u8, sa, sb)) {
            std.debug.print("Found difference at [{}, {}]\n", .{ ix, ix + size });
        }

        ix += size;
    }
}
