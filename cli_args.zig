const std = @import("std");

test "ut" {
    for (std.os.argv) |arg| {
        std.debug.print("arg: {s}\n", .{arg});
    }
}
