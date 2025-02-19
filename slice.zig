test "array to slice" {
    const extensions = [_][]const u8{ ".o", ".exe" };
    // First const is necessary to support assignment from an array
    var sl: []const []const u8 = undefined;
    sl = &extensions;
}
