const std = @import("std");

fn arrayLenOf(T: type) usize {
    return switch (@typeInfo(T)) {
        .array => |a| a.len,
        else => @compileError("Expected T to be an array"),
    };
}

test "array" {
    const ut = std.testing;

    const Array = [10]u8;

    try ut.expectEqual(10, arrayLenOf(Array));

    const array: Array = undefined;
    try ut.expectEqual(10, array.len);
}
