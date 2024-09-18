#[derive(Default)]
pub struct Object {
    a: usize,
    str: String,
}

impl Object {
    pub fn build(cb: impl FnOnce(&mut Object) -> ()) -> Object {
        let mut ret = Object::default();
        cb(&mut ret);
        ret.init();
        ret
    }

    fn init(&mut self) {
        self.str = format!("{}", self.a);
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_build() {
        let object = Object::build(|o| o.a = 42);
        assert_eq!(object.a, 42);
        assert_eq!(object.str, "42");
    }
}
