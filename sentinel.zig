const std = @import("std");

test "sentinel" {
    var backing = [_:0]u8{};
    var buf: [:0]u8 = &backing;
    std.debug.print("buf {}\n", .{buf.len});
}
