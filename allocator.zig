const std = @import("std");
const ut = std.testing;

const Data = struct {
    const Strings = std.ArrayList([]const u8);

    strings: Strings = undefined,

    aa: std.heap.ArenaAllocator,

    pub fn init(a: std.mem.Allocator) Data {
        var options = Data{ .aa = std.heap.ArenaAllocator.init(a) };

        options.strings = Strings.empty;

        return options;
    }
    pub fn deinit(self: Data) void {
        std.debug.print("Data.deinit()\n", .{});
        self.aa.deinit();
    }

    pub fn parse(self: *Data) !void {
        std.debug.print("strings: {}\n", .{self.strings});
        try self.strings.append(self.aa.allocator(), "abc");
    }
};

test "allocator" {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();

    const a = gpa.allocator();

    var data = Data.init(a);
    defer data.deinit();

    try data.parse();
}
