const std = @import("std");
const ut = std.testing;

fn read(file: std.Io.File, caption: []const u8) !void {
    var rbuf: [100]u8 = undefined;
    var reader = file.reader(ut.io, &rbuf);

    while (reader.interface.peekGreedy(1)) |str| {
        defer reader.interface.tossBuffered();
        std.debug.print("{s}:{}=>{s}\n", .{ caption, str.len, str });
    } else |_| {}
}

test "process" {
    const cmd = [_][]const u8{"rake"};
    const dir_fp = "/home/geertf/am";

    var proc = try std.process.spawn(ut.io, .{
        .argv = &cmd,
        .cwd = dir_fp,
        .stdout = .pipe,
        .stderr = .pipe,
    });
    defer proc.kill(ut.io);

    if (proc.stdout) |stdout|
        try read(stdout, "stdout");
    if (proc.stderr) |stderr|
        try read(stderr, "stderr");

    const term = try proc.wait(ut.io);
    std.debug.print("term: {}\n", .{term});
}
