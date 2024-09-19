fn f() -> anyhow::Result<()> {
    std::fs::read("does not exist")?;
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_anyhow() -> anyhow::Result<()> {
        f()?;
        Ok(())
    }
}
