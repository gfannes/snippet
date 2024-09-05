pub type Range = std::ops::Range<usize>;

#[derive(Clone)]
pub struct Strange<'a> {
    all: &'a str,
    safepoint: &'a str,
    rest: &'a str,
}

impl<'a> Strange<'a> {
    pub fn new(content: &str) -> Strange {
        Strange {
            all: content,
            safepoint: content,
            rest: content,
        }
    }

    pub fn to_str(&self) -> &str {
        self.rest
    }

    pub fn read_char(&mut self) -> bool {
        self.read_char_opt().is_some()
    }
    pub fn read_char_opt(&mut self) -> Option<char> {
        split_first(self.rest).and_then(|(ch, rest)| {
            self.rest = rest;
            Some(ch)
        })
    }

    pub fn read_char_if(&mut self, wanted: char) -> bool {
        self.read_char_if_opt(wanted).is_some()
    }
    pub fn read_char_if_opt(&mut self, wanted: char) -> Option<char> {
        split_first(self.rest).and_then(|(ch, rest)| {
            (ch == wanted).then(|| {
                self.rest = rest;
                ch
            })
        })
    }

    pub fn reset(&mut self) {
        self.rest = self.safepoint;
    }

    pub fn pop_str(&mut self) -> &str {
        let ret = &self.safepoint[..distance(self.safepoint, self.rest)];
        self.safepoint = self.rest;
        ret
    }
    pub fn pop_range(&mut self) -> Range {
        let ret = Range {
            start: distance(self.all, self.safepoint),
            end: distance(self.all, self.rest),
        };
        self.safepoint = self.rest;
        ret
    }
}

fn split_first<'a>(content: &'a str) -> Option<(char, &'a str)> {
    content
        .chars()
        .next()
        .and_then(|ch| Some((ch, &content[ch.len_utf8()..])))
}

fn distance(from: &str, to: &str) -> usize {
    to.as_ptr() as usize - from.as_ptr() as usize
}

pub fn main() {}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_scn() {
        let s = Strange::new("abc");
        assert_eq!(s.to_str(), "abc");

        {
            let mut s = s.clone();
            if s.read_char_if('a') && s.read_char_if('b') {
                assert_eq!(s.pop_str(), "ab");
            }
            assert_eq!(s.to_str(), "c");
        }

        {
            let mut s = s.clone();
            if s.read_char_if('a') && s.read_char_if('b') {
                assert_eq!(s.pop_range(), 0..2);
            }
            assert_eq!(s.to_str(), "c");
        }

        {
            let mut s = s.clone();
            if s.read_char_if('a') && s.read_char_if('b') {
                s.reset();
            }
            assert_eq!(s.to_str(), "abc");
        }

        {
            let mut s = s.clone();
            s.read_char_if_opt('a')
                .and_then(|_| s.read_char_if_opt('b'));
        }
    }

    #[test]
    fn test_perf() {
        let len = 1024 * 1024 * 1024;
        let content = "a".repeat(len);
        let mut s = Strange::new(&content);
        while s.read_char_if('a') {
            s.pop_str();
        }
        assert_eq!(s.to_str(), "");
    }
}
