const std = @import("std");
const ut = std.testing;

test "cwd" {
    const cwd = try std.fs.cwd().realpathAlloc(ut.allocator, ".");
    defer ut.allocator.free(cwd);
    std.debug.print("cwd: {s}\n", .{cwd});
}
