use std::io;

use ratatui::{
    crossterm::event::{self, KeyCode, KeyEventKind},
    style::Stylize,
    widgets::Paragraph,
    DefaultTerminal,
};
// use ratatui::style::{Color, Style};
// use ratatui::terminal::{self, Terminal};

fn run(mut terminal: DefaultTerminal) -> io::Result<()> {
    terminal.
    loop {
        terminal.draw(|frame| {
            let greeting = Paragraph::new("Hello Ratatui! (press 'q' to quit)")
                .white()
                .on_blue();
            frame.render_widget(greeting, frame.area());
        })?;

        if let event::Event::Key(key) = event::read()? {
            if key.kind == KeyEventKind::Press && key.code == KeyCode::Char('q') {
                return Ok(());
            }
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_hello_world() -> Result<(), std::io::Error> {
        let mut terminal = ratatui::init();
        terminal.clear()?;
        let app_result = run(terminal);
        ratatui::restore();
        app_result

        // let mut terminal = Terminal::new()?;
        // terminal.write(
        //     "Hello, world!",
        //     Style::default().fg(Color::Yellow).bg(Color::Black),
        // )?;
        // terminal.flush()?;

        // Ok(())
    }
}
