const std = @import("std");

test "localhost" {
    const ut = std.testing;

    var cb = struct {
        const Cb = @This();

        io: std.Io,
        port: u16 = 1234,
        ipaddr: ?std.Io.net.IpAddress = null,
        client_thread: ?std.Thread = null,
        server_thread: ?std.Thread = null,

        fn init(cb: *Cb) !void {
            cb.ipaddr = try std.Io.net.IpAddress.parse("0.0.0.0", cb.port);
            cb.client_thread = try std.Thread.spawn(.{}, client_func, .{cb});
            cb.server_thread = try std.Thread.spawn(.{}, server_func, .{cb});
        }
        fn deinit(cb: *Cb) void {
            if (cb.client_thread) |thread|
                thread.join();
            if (cb.server_thread) |thread|
                thread.join();
        }
        fn server_func(cb: *Cb) !void {
            if (cb.ipaddr) |ipaddr| {
                std.debug.print("Server will listen to {any}\n", .{ipaddr});
                var server = try ipaddr.listen(cb.io, .{ .reuse_address = true });
                defer server.deinit(cb.io);

                std.debug.print("Waiting for connection\n", .{});
                const stream = try server.accept(cb.io);
                defer stream.close(cb.io);
                std.debug.print("\tServer is connected\n", .{});

                var rb: [10]u8 = undefined;
                var reader = stream.reader(cb.io, &rb);
                while (reader.interface.takeByte()) |byte| {
                    std.debug.print("Server received '{c}' ({})\n", .{byte, byte});
                } else |_| {
                }
            }
        }
        fn client_func(cb: *Cb) !void {
            if (cb.ipaddr) |ipaddr| {
                std.debug.print("Client is connecting to {any}\n", .{ipaddr});
                const stream = try ipaddr.connect(cb.io, .{ .mode = .stream });
                defer stream.close(cb.io);
                std.debug.print("\tClient is connected\n", .{});

                var wb: [10]u8 = undefined;
                var writer = stream.writer(cb.io, &wb);
                try writer.interface.print("Hello from client\n", .{});
                try writer.interface.flush();
            }
        }
    }{ .io = ut.io };
    defer cb.deinit();
    try cb.init();
}
