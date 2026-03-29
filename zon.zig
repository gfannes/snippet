const std = @import("std");

const A = struct {
    // Defaulted fields can be omitted in zon
    a: i32 = 0,
    array: []i32 = &.{},
};

test "zon" {
    const ut = std.testing;

    const content = ".{.array = .{1,2,3}}";

    var diag: std.zon.parse.Diagnostics = .{};
    defer diag.deinit(ut.allocator);

    if (std.zon.parse.fromSliceAlloc(A, ut.allocator, content, &diag, .{})) |a| {
        defer std.zon.parse.free(ut.allocator, a);
        std.debug.print("{}\n", .{a});
    } else |err| {
        std.debug.print("error: {} {f}\n", .{ err, diag });
    }
}
