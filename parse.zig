const std = @import("std");

test "parse" {
    const ut = std.testing;

    if (std.fmt.parseInt(u16, "3", 10)) |v|
        try ut.expectEqual(3, v)
    else |_|
        try ut.expect(false);
}
