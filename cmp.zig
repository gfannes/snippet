const std = @import("std");

const File = struct {
    const Self = @This();

    io: std.Io,

    buffer: [1024 * 1024]u8 = undefined,

    file: std.Io.File = undefined,
    stat: std.Io.File.Stat = undefined,
    rbuf: [1024]u8 = undefined,
    reader: std.Io.File.Reader = undefined,

    fn init(self: *Self, path: []const u8) !void {
        self.file = try std.Io.Dir.openFileAbsolute(self.io, path, .{});
        self.stat = try self.file.stat(self.io);
        self.reader = self.file.reader(self.io, &self.rbuf);
    }
    fn deinit(self: *Self) void {
        self.file.close(self.io);
    }
};

fn compare(io: std.Io, a_fp: []const u8, b_fp: []const u8) !bool {
    var fa = File{ .io = io };
    try fa.init(a_fp);
    defer fa.deinit();

    var fb = File{ .io = io };
    try fb.init(b_fp);
    defer fb.deinit();

    if (fa.stat.size != fb.stat.size)
        return false;

    var ix: usize = 0;
    while (ix < fa.stat.size) {
        const size = @min(fa.buffer.len, fa.stat.size - ix);

        const sa = fa.buffer[0..size];
        try fa.reader.interface.readSliceAll(sa);

        const sb = fb.buffer[0..size];
        try fb.reader.interface.readSliceAll(sb);

        if (!std.mem.eql(u8, sa, sb)) {
            std.debug.print("Found difference at [{}, {}]\n", .{ ix, ix + size });
        }

        ix += size;
    }

    return true;
}

test "cmp" {
    const ut = std.testing;

    const ok = compare(ut.io, "/home/geertf/aws/auro-masv-transfer/AURO TEST DISC/AURO_TESTDISC_V10.iso", "/home/geertf/story/cd13/AURO_TESTDISC_V10.iso") catch true;
    try ut.expect(ok);
}
