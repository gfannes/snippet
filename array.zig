const std = @import("std");

test "array" {
    const ut = std.testing;

    const Array = [10]u8;

    const array: Array = undefined;
    try ut.expectEqual(10, array.len);
}
