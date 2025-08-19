const std = @import("std");

const Color = enum { red, green, blue };

test "formatting" {
    const c = Color.green;

    // ┌──── fill character (default ' ')
    // │┌─── alignment  (< left, > right, ^ center; default is right)
    // ││┌── field width (must be an integer literal)
    // {:>10}   right-align inside a field 10 chars wide
    std.debug.print("'{:>100}'\n", .{c}); // prints '     green'

    std.debug.print("'{:<100}'\n", .{c}); // prints 'green     '

    std.debug.print("'{:^100}'\n", .{c}); // prints '  green   '

    // You can change the fill character by writing it *before* the alignment flag.
    std.debug.print("'{:*>100}'\n", .{c}); // prints '*****green'

    // If you want the *numeric* value instead of the tag name,
    // cast through @intFromEnum(c) and format as usual:
    std.debug.print("numeric: {d:02}\n", .{@intFromEnum(c)}); // 'numeric: 01'
}
