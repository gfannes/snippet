const std = @import("std");

fn read(file: std.fs.File, caption: []const u8) !void {
    var buf: [16]u8 = undefined;
    while (true) {
        const n = try file.readAll(&buf);
        if (n == 0)
            break;
        std.debug.print("{s}:{}=>{s}\n", .{ caption, n, buf[0..n] });
    }
}

test "process" {
    const ut = std.testing;

    const cmd = [_][]const u8{"rake"};

    var proc = std.process.Child.init(&cmd, ut.allocator);
    proc.stdout_behavior = .Pipe;
    proc.stderr_behavior = .Ignore;

    const dir_fp = "/home/geertf/am";
    var dir = try std.fs.openDirAbsolute(dir_fp, .{});
    defer dir.close();
    proc.cwd_dir = dir;

    try proc.spawn();

    if (proc.stdout) |stdout|
        try read(stdout, "stdout");
    if (proc.stderr) |stderr|
        try read(stderr, "stderr");

    const term = try proc.wait();
    std.debug.print("term: {}\n", .{term});
}
