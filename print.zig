const std = @import("std");

const Point = struct {
    x: i32,
    y: i32,
};
const Node = struct {
    data: i64,
};
const Edge = struct {
    from: *Node,
    to: *Node,
};

test "ut" {
    std.debug.print("42: {}\n", .{42});

    const p: Point = .{ .x = 1, .y = 2 };
    std.debug.print("p: {}\n", .{p});

    var a: Node = .{ .data = 0 };
    var b: Node = .{ .data = 1 };
    const edge: Edge = .{ .from = &a, .to = &b };
    std.debug.print("edge: {}\n", .{edge});
}
