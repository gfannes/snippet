const std = @import("std");

pub fn main(init: std.process.Init) !void {
    for (init.minimal.args) |arg| {
        std.debug.print("arg: {s}\n", .{arg});
    }
}
