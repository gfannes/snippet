const std = @import("std");
const ut = std.testing;

const Data = struct {
    const Strings = std.ArrayList([]const u8);

    strings: Strings = undefined,

    _aa: std.heap.ArenaAllocator,

    pub fn init(ma: std.mem.Allocator) Data {
        var options = Data{ ._aa = std.heap.ArenaAllocator.init(ma) };

        options.strings = Strings.init(options._aa.allocator());

        return options;
    }
    pub fn deinit(self: Data) void {
        std.debug.print("Data.deinit()\n", .{});
        self._aa.deinit();
    }

    pub fn parse(self: *Data) !void {
        std.debug.print("strings: {}\n", .{self.strings});
        try self.strings.append("abc");
    }
};

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();

    const ma = gpa.allocator();

    var data = Data.init(ma);
    defer data.deinit();

    try data.parse();
}
