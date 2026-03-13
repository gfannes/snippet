const std = @import("std");
const ut = std.testing;

test "cwd" {
    // Using std.Io.Dir.cwd() from a test requires you to first open this folder explicitly
    var cwd = try std.Io.Dir.cwd().openDir(ut.io, ".", .{ .iterate = true });

    var buffer: [std.fs.max_path_bytes]u8 = undefined;
    const size = try cwd.realPath(ut.io, &buffer);
    const cwd_str = buffer[0..size];

    std.debug.print("cwd: {s}\n", .{cwd_str});
}
