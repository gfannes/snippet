const std = @import("std");

test "io" {
    const ut = std.testing;

    var al = std.ArrayList(u8){};
    defer al.deinit(ut.allocator);
    var w = std.Io.Writer.Allocating.fromArrayList(ut.allocator, &al);

    try w.writer.print("test", .{});
    try w.writer.flush();

    al = w.toArrayList();

    try ut.expectEqual(4, al.items.len);
}
