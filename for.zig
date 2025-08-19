const std = @import("std");

test "for" {
    // const ut = std.testing;

    for (10..15) |ix| {
        std.debug.print("{}\n", .{ix});
    }
}
