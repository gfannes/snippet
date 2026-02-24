const std = @import("std");

test "time" {
    // const ut = std.testing;

    const time = try std.posix.clock_gettime(.REALTIME);
    std.debug.print("{any}\n", .{time});

    const esecs = std.time.epoch.EpochSeconds{ .secs = @intCast(time.sec) };
    std.debug.print("esecs: {any}\n", .{esecs});
    const eday = esecs.getEpochDay();
    std.debug.print("eday: {any}\n", .{eday});
    const yd = eday.calculateYearDay();
    std.debug.print("year: {}, day: {}\n", .{ yd.year, yd.day });
    const md = yd.calculateMonthDay();
    std.debug.print("month: {}, day: {}\n", .{ md.month, md.day_index + 1 });
}
