const std = @import("std");

const S = struct {
    b: bool,
    i: i32,
};

// Converts T into a struct with the same fields, except that each field has an additional optional layer.
fn Optionalized(comptime T: type) type {
    const ti = @typeInfo(T);
    if (ti != .@"struct")
        @compileError("Optionalized expects a struct type");

    const s = ti.@"struct";

    var names: [s.fields.len][]const u8 = undefined;
    var types: [s.fields.len]type = undefined;
    var attrs: [s.fields.len]std.builtin.Type.StructField.Attributes = undefined;

    inline for (s.fields, 0..) |field, i| {
        const OptT = ?field.type;

        names[i] = field.name;
        types[i] = OptT;
        attrs[i] = .{
            .@"comptime" = field.is_comptime,
            .@"align" = if (field.alignment == @alignOf(field.type))
                null
            else
                field.alignment,
            .default_value_ptr = nullDefaultPtr(OptT),
        };
    }

    return @Struct(s.layout, s.backing_integer, &names, &types, &attrs);
}

fn nullDefaultPtr(comptime T: type) *const anyopaque {
    return &struct {
        const value: T = null;
    }.value;
}

const OptS = Optionalized(S);

test "optstruct" {
    const ut = std.testing;
    try ut.expect(true);

    const opts: OptS = .{};
    std.debug.print("{}\n", .{opts});
}
