const std = @import("std");

pub fn main() void {
    for (std.os.argv) |arg| {
        std.debug.print("arg: {s}\n", .{arg});
    }
}
