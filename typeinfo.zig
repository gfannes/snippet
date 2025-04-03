const std = @import("std");

const S = struct {
    i: i32 = 0,
    maybe_i: ?i32 = null,
};

fn f(v: anytype) void {
    std.debug.print("{}\n", .{@typeInfo(@TypeOf(v.*))});
    v.* = 42;
}

test "typeinfo" {
    const ut = std.testing;
    try ut.expect(true);

    var s = S{};
    std.debug.print("{}\n", .{s.i});
    f(&s.i);
    std.debug.print("{}\n", .{s.i});
    f(&s.maybe_i);

    std.debug.print("{}\n", .{@typeInfo(@TypeOf(null))});
}
