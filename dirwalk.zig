const std = @import("std");
const ut = std.testing;

test "std.fs.Dir.iterate" {
    var dir = try std.Io.Dir.cwd().openDir(ut.io, ".", .{ .iterate = true });
    defer dir.close(ut.io);

    var it = dir.iterate();
    while (try it.next(ut.io)) |el| {
        std.debug.print("{s} {}\n", .{ el.name, el.kind });
    }
}

test "std.fs.Dir.walk" {
    var dir = try std.Io.Dir.cwd().openDir(ut.io, ".", .{ .iterate = true });
    defer dir.close(ut.io);

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const ma = gpa.allocator();
    var walker = try dir.walk(ma);
    while (try walker.next(ut.io)) |el| {
        std.debug.print("{s} {s} {}\n", .{ el.basename, el.path, el.kind });
    }
}

const Walker = struct {
    const Filter = struct {
        hidden: bool = true,
        // First const is necessary to support assignment from `&[][]const u8{".o", ".exe"}`.
        extensions: []const []const u8 = &.{},
    };

    io: std.Io,

    filter: Filter = .{},
    _buffer: [std.fs.max_path_bytes]u8 = undefined,

    pub fn walk(self: *Walker, dir: std.Io.Dir, cb: anytype) !void {
        var it = dir.iterate();
        while (try it.next(self.io)) |el| {
            if (self._filter(dir, el)) {
                switch (el.kind) {
                    std.Io.File.Kind.file => {
                        const size = try dir.realPath(self.io, &self._buffer);
                        const path = self._buffer[0..size];
                        cb(path, el.name);
                    },
                    std.Io.File.Kind.directory => {
                        var subdir = try dir.openDir(self.io, el.name, .{ .iterate = true });
                        defer subdir.close(self.io);
                        try self.walk(subdir, cb);
                    },
                    else => {},
                }
            }
        }
    }

    fn _filter(self: Walker, _: std.Io.Dir, entry: std.Io.Dir.Entry) bool {
        if (self.filter.hidden and entry.name.len > 0 and entry.name[0] == '.')
            return false;

        const my_ext = std.fs.path.extension(entry.name);
        for (self.filter.extensions) |ext| {
            if (std.mem.eql(u8, my_ext, ext))
                return false;
        }

        return true;
    }
};

test "walk" {
    var walker = Walker{ .io = ut.io, .filter = .{ .extensions = &[_][]const u8{ ".o", ".exe" } } };
    const cb = struct {
        pub fn call(path: []const u8, name: []const u8) void {
            std.debug.print("path: {s}, name: {s}\n", .{ path, name });
        }
    };
    var dir = try std.Io.Dir.cwd().openDir(ut.io, ".", .{ .iterate = true });
    defer dir.close(ut.io);
    try walker.walk(dir, cb.call);
}
