struct Filter {
    coeffs: Vec<f32>,
    state: Vec<f32>,
    start_ix: usize,
}
impl Filter {
    fn new() -> Filter {
        let coeffs = vec![0.1, 0.2, 0.3];
        let mut state = Vec::new();
        state.resize(coeffs.len(), 0.0);
        Filter {
            coeffs,
            state,
            start_ix: 0,
        }
    }
    fn call(&mut self, v: f32) -> f32 {
        for (ix0, coeff) in self.coeffs.iter().enumerate() {
            let x = coeff * v;
            let state_ix = (self.start_ix + self.state.len() + ix0) % self.state.len();
            self.state[state_ix] += x;
        }
        let ix = self.start_ix % self.state.len();
        self.start_ix += 1;
        let res = self.state[ix];
        self.state[ix] = 0.0;
        res
    }
}

fn main() {
    let mut filter = Filter::new();
    println!("{}", filter.call(0.0));
    println!("{}", filter.call(0.0));
    println!("{}", filter.call(0.0));
    println!("{}", filter.call(0.0));
    println!("{}", filter.call(1.0));
    println!("{}", filter.call(0.0));
    println!("{}", filter.call(0.0));
    println!("{}", filter.call(0.0));
    println!("{}", filter.call(0.0));
}
