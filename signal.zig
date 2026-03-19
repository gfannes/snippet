const std = @import("std");

var stop = false;

fn onSigInt(_: std.posix.SIG) callconv(.c) void {
    stop = true;
}

pub fn main(init: std.process.Init) !void {
    const action = std.posix.Sigaction{
        .handler = .{ .handler = onSigInt },
        .mask = std.posix.sigemptyset(),
        .flags = 0,
    };

    std.posix.sigaction(std.posix.SIG.INT, &action, null);

    while (!stop) {
        std.debug.print("working...\n", .{});
        try std.Io.sleep(init.io, std.Io.Duration.fromMilliseconds(100), .real);
    }

    std.debug.print("Ctrl-C received, exiting cleanly.\n", .{});
}
