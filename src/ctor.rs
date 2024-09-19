struct S {
    i: i32,
}

impl S {
    pub fn new() -> S {
        S { i: 42 }
    }
    pub fn now() -> S {
        Self::new()
    }
    pub fn naw() -> S {
        S::new()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_new() {
        let s = S::new();
    }

    #[test]
    fn test_now() {
        let s = S::now();
    }

    #[test]
    fn test_naw() {
        let s = S::naw();
    }
}
