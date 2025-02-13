fn maybe() !i32 {
    return null;
}

fn doAThing(_: []u8) void {
    const number = maybe() catch 13;
    _ = number; // ...
}

test "ut" {}
