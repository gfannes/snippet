const std = @import("std");
const ut = std.testing;

test "time" {
    const time_sec = std.Io.Clock.now(.real, ut.io).toSeconds();
    std.debug.print("time_sec: {}\n", .{time_sec});

    const esecs = std.time.epoch.EpochSeconds{ .secs = @intCast(time_sec) };
    std.debug.print("esecs: {any}\n", .{esecs});
    const eday = esecs.getEpochDay();
    std.debug.print("eday: {any}\n", .{eday});
    const yd = eday.calculateYearDay();
    std.debug.print("year: {}, day: {}\n", .{ yd.year, yd.day });
    const md = yd.calculateMonthDay();
    std.debug.print("month: {}, day: {}\n", .{ md.month, md.day_index + 1 });
}
