const std = @import("std");

pub fn main() !void {
    var counter = struct {
        count: usize,

        fn increment(self: *@This()) usize {
            self.count += 1;
            return self.count;
        }
    }{ .count = 0 };

    // Function pointer to simulate a stateful lambda
    const lambda = @TypeOf(counter).increment;

    // Call function with the stateful struct
    std.debug.print("Count: {}\n", .{lambda(&counter)});
    std.debug.print("Count: {}\n", .{lambda(&counter)});
}
