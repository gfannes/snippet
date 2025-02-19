const std = @import("std");
const ut = std.testing;

test "iterate" {
    // var dir = try std.fs.cwd().openDir(".", .{ .iterate = true });
    // defer dir.close();

    // var it = dir.iterate();
    // while (try it.next()) |el| {
    //     std.debug.print("{s}\n", .{el.name});
    // }
}

test "walk" {
    var dir = try std.fs.cwd().openDir(".", .{ .iterate = true });
    defer dir.close();

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const ma = gpa.allocator();
    var walker = try dir.walk(ma);
    while (try walker.next()) |el| {
        std.debug.print("{s} {s} {}\n", .{ el.basename, el.path, el.kind });
    }
}
