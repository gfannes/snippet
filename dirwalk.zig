const std = @import("std");
const ut = std.testing;

test "ut" {
    var dir = try std.fs.cwd().openDir(".", .{ .iterate = true });
    defer dir.close();

    var it = dir.iterate();
    while (try it.next()) |el| {
        std.debug.print("{s}\n", .{el.name});
    }
}
