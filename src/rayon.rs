use rayon::prelude::*;

#[derive(Debug)]
struct Task {
    content: String,
    checksum: Option<u64>,
}

impl Task {
    fn new(content: &str) -> Task {
        Task {
            content: content.into(),
            checksum: None,
        }
    }
    fn compute(&mut self) {
        let mut checksum: u64 = 0;
        for _ in 0..10000000 {
            for ch in self.content.chars() {
                checksum = checksum.wrapping_mul(11);
                checksum = checksum.wrapping_add(u64::from(ch));
            }
        }
        self.checksum = Some(checksum);
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_par_iter() {
        let mut tasks = [
            Task::new("abc"),
            Task::new("def"),
            Task::new("ghi"),
            Task::new("jkl"),
            Task::new("mno"),
            Task::new("abc"),
            Task::new("def"),
            Task::new("ghi"),
            Task::new("jkl"),
            Task::new("mno"),
            Task::new("abc"),
            Task::new("def"),
            Task::new("ghi"),
            Task::new("jkl"),
            Task::new("mno"),
            Task::new("abc"),
            Task::new("def"),
            Task::new("ghi"),
            Task::new("jkl"),
            Task::new("mno"),
            Task::new("abc"),
            Task::new("def"),
            Task::new("ghi"),
            Task::new("jkl"),
            Task::new("mno"),
            Task::new("abc"),
            Task::new("def"),
            Task::new("ghi"),
            Task::new("jkl"),
            Task::new("mno"),
            Task::new("abc"),
            Task::new("def"),
            Task::new("ghi"),
            Task::new("jkl"),
            Task::new("mno"),
            Task::new("abc"),
            Task::new("def"),
            Task::new("ghi"),
            Task::new("jkl"),
            Task::new("mno"),
            Task::new("abc"),
            Task::new("def"),
            Task::new("ghi"),
            Task::new("jkl"),
            Task::new("mno"),
            Task::new("abc"),
            Task::new("def"),
            Task::new("ghi"),
            Task::new("jkl"),
            Task::new("mno"),
            Task::new("abc"),
            Task::new("def"),
            Task::new("ghi"),
            Task::new("jkl"),
            Task::new("mno"),
            Task::new("abc"),
            Task::new("def"),
            Task::new("ghi"),
            Task::new("jkl"),
            Task::new("mno"),
            Task::new("abc"),
            Task::new("def"),
            Task::new("ghi"),
            Task::new("jkl"),
            Task::new("mno"),
            Task::new("abc"),
            Task::new("def"),
            Task::new("ghi"),
            Task::new("jkl"),
            Task::new("mno"),
            Task::new("abc"),
            Task::new("def"),
            Task::new("ghi"),
            Task::new("jkl"),
            Task::new("mno"),
            Task::new("abc"),
            Task::new("def"),
            Task::new("ghi"),
            Task::new("jkl"),
            Task::new("mno"),
        ];

        tasks.par_iter_mut().for_each(|task| task.compute());
        // tasks.iter_mut().for_each(|task| task.compute());

        println!("{:?}", &tasks);
    }
}
