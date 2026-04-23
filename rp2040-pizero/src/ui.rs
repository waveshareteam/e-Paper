use crate::data::Detail;
use embedded_graphics::{
    pixelcolor::BinaryColor,
    prelude::*,
    primitives::{Circle, PrimitiveStyleBuilder},
    text::Text,
};
use u8g2_fonts::{U8g2TextStyle, fonts};
use hypher::{hyphenate, Lang};
use heapless::String;
use core::fmt::Write;

// Physical panel dimensions
pub const PHYS_WIDTH: u32 = 176;
pub const PHYS_HEIGHT: u32 = 264;

// Logical dimensions after 90° CW rotation
pub const WIDTH: u32 = PHYS_HEIGHT; // 264
pub const HEIGHT: u32 = PHYS_WIDTH; // 176

// Rendering Offsets
const TITLE_SEP_Y: i32 = 20; // Margin below title block baseline
const MENU_TEXT_X_SELECTED: i32 = 14;
const MENU_TEXT_X_NORMAL: i32 = 2;
const MENU_WRAP_OFFSET_X: i32 = 6;
const MENU_LINE_HEIGHT: i32 = 15;

const PAGE_CONT_START_Y: i32 = 16;
const BODY_LINE_HEIGHT_P0: i32 = 15;
const BODY_LINE_HEIGHT_P1: i32 = 16;

pub struct DisplayBuffer {
    pub buffer: [u8; (PHYS_WIDTH * PHYS_HEIGHT / 8) as usize],
}

impl Default for DisplayBuffer {
    fn default() -> Self {
        Self {
            buffer: [0xFF; (PHYS_WIDTH * PHYS_HEIGHT / 8) as usize],
        }
    }
}

impl DisplayBuffer {
    pub fn new() -> Self {
        Self::default()
    }

    pub fn clear(&mut self) {
        self.buffer.fill(0xFF);
    }
}

impl DrawTarget for DisplayBuffer {
    type Color = BinaryColor;
    type Error = core::convert::Infallible;

    fn draw_iter<I>(&mut self, pixels: I) -> Result<(), Self::Error>
    where
        I: IntoIterator<Item = Pixel<Self::Color>>,
    {
        for Pixel(point, color) in pixels.into_iter() {
            if point.x >= 0 && point.x < WIDTH as i32 && point.y >= 0 && point.y < HEIGHT as i32 {
                let phys_x = (PHYS_WIDTH as i32 - 1 - point.y) as usize;
                let phys_y = point.x as usize;
                let idx = (phys_x / 8) + (phys_y * (PHYS_WIDTH as usize / 8));
                let bit = 7 - (phys_x % 8);
                if color == BinaryColor::Off {
                    self.buffer[idx] &= !(1 << bit);
                } else {
                    self.buffer[idx] |= 1 << bit;
                }
            }
        }
        Ok(())
    }
}

impl OriginDimensions for DisplayBuffer {
    fn size(&self) -> Size {
        Size::new(WIDTH, HEIGHT)
    }
}

#[derive(Default)]
pub struct Ui {
    pub buffer: DisplayBuffer,
}

impl Ui {
    pub fn new() -> Self {
        Self::default()
    }

    fn body_style() -> U8g2TextStyle<BinaryColor> {
        U8g2TextStyle::new(fonts::u8g2_font_ncenR12_tr, BinaryColor::Off)
    }

    fn bold_style() -> U8g2TextStyle<BinaryColor> {
        U8g2TextStyle::new(fonts::u8g2_font_ncenB12_tr, BinaryColor::Off)
    }

    fn title_style() -> U8g2TextStyle<BinaryColor> {
        U8g2TextStyle::new(fonts::u8g2_font_ncenB14_tr, BinaryColor::Off)
    }

    fn get_ascent(style: &U8g2TextStyle<BinaryColor>) -> i32 {
        // The y-offset of the bounding box at Point(0,0) gives the ascent.
        // We use "A" as a reference for typical capitalized height.
        Text::new("A", Point::zero(), style.clone())
            .bounding_box()
            .top_left
            .y
            .abs()
    }

    fn get_line_height(style: &U8g2TextStyle<BinaryColor>) -> i32 {
        // Total height of a typical line including a bit of lead
        let bb = Text::new("Ag!", Point::zero(), style.clone()).bounding_box();
        bb.size.height as i32
    }

    /// Draws the title, wrapping if necessary. Returns the total height used in pixels.
    fn draw_title(&mut self, title: &str, max_width: i32) -> i32 {
        let style = Self::title_style();
        let line_height = Self::get_line_height(&style);
        let mut y = Self::get_ascent(&style); // Dynamic tight top baseline
        let mut remaining = title;

        while !remaining.is_empty() {
            let (first, rest) = self.wrap_title_text(remaining, max_width);
            let _ = Text::new(first, Point::new(0, y), Self::title_style()).draw(&mut self.buffer);
            remaining = rest;
            if !remaining.is_empty() {
                y += line_height;
            }
        }
        y + TITLE_SEP_Y
    }

    fn wrap_title_text<'a>(&self, text: &'a str, max_width: i32) -> (&'a str, &'a str) {
        let mut last_valid_split = 0;
        for (i, c) in text.char_indices() {
            let width = Text::new(
                &text[..i + c.len_utf8()],
                Point::zero(),
                Self::title_style(),
            )
            .bounding_box()
            .size
            .width as i32;
            if width > max_width {
                if last_valid_split == 0 {
                    return (&text[..i], &text[i..]);
                }
                return (
                    &text[..last_valid_split],
                    text[last_valid_split..].trim_start(),
                );
            }
            if c.is_whitespace() {
                last_valid_split = i;
            }
        }
        (text, "")
    }

    pub fn measure(&self, text: &str, is_bold: bool) -> i32 {
        let style = if is_bold {
            Self::bold_style()
        } else {
            Self::body_style()
        };
        Text::new(text, Point::zero(), style)
            .bounding_box()
            .size
            .width as i32
    }

    fn menu_text_x(&self, is_selected: bool) -> i32 {
        if is_selected {
            MENU_TEXT_X_SELECTED
        } else {
            MENU_TEXT_X_NORMAL
        }
    }

    fn draw_menu_item(
        &mut self,
        item: &str,
        is_selected: bool,
        y: i32,
        max_width: i32,
        line_height: i32,
    ) -> i32 {
        let is_bold = item.starts_with('#');

        let display_text = if is_bold {
            let skip = item.chars().next().map_or(1, |c| c.len_utf8());
            &item[skip..]
        } else {
            item
        };

        let text_x = self.menu_text_x(is_selected);
        let wrap_x = text_x + MENU_WRAP_OFFSET_X;
        let avail = max_width - text_x;

        if is_selected {
            let bul_style = PrimitiveStyleBuilder::new()
                .fill_color(BinaryColor::Off)
                .build();
            let _ = Circle::new(Point::new(4, y - 8), 5)
                .into_styled(bul_style)
                .draw(&mut self.buffer);
        }

        if self.measure(item, is_bold) <= avail {
            let _ = Text::new(
                display_text,
                Point::new(text_x, y),
                if is_bold {
                    Self::bold_style()
                } else {
                    Self::body_style()
                },
            )
            .draw(&mut self.buffer);
            return line_height;
        }

        // Word wrap
        let (l1, l2, _) = self.wrap_text(item, avail);
        let _ = Text::new(
            l1,
            Point::new(text_x, y),
            if is_bold {
                Self::bold_style()
            } else {
                Self::body_style()
            },
        )
        .draw(&mut self.buffer);
        if !l2.is_empty() {
            let _ = Text::new(
                l2,
                Point::new(wrap_x, y + line_height),
                if is_bold {
                    Self::bold_style()
                } else {
                    Self::body_style()
                },
            )
            .draw(&mut self.buffer);
            line_height * 2
        } else {
            line_height
        }
    }

    fn wrap_text<'a>(&self, text: &'a str, max_width: i32) -> (&'a str, &'a str, bool) {
        let is_bold = text.starts_with('#');
        let skip = if is_bold {
            text.chars().next().map_or(1, |c| c.len_utf8())
        } else {
            0
        };
        let content = &text[skip..];

        let mut last_valid_split = 0;

        for (i, c) in content.char_indices() {
            let test_str = &text[..skip + i + c.len_utf8()];
            let width = self.measure(test_str, is_bold);
            if width > max_width {
                // Potential split point. Check if we can hyphenate the current word.
                let word_start = last_valid_split;
                let word_end = i;
                let word = &content[word_start..word_end];
                
                if !is_bold && word.len() >= 5 {
                    let word_trimmed = word.trim_start();
                    let trim_offset = word.len() - word_trimmed.len();
                    
                    let mut best_hyphen_idx = 0;
                    let mut current_prefix_len = 0;
                    let prefix_width_at_start = if word_start == 0 { 0 } else { self.measure(&text[..skip + word_start], is_bold) };
                    
                    for sub in hyphenate(word_trimmed, Lang::English) {
                        current_prefix_len += sub.len();
                        let test_str = &word[..trim_offset + current_prefix_len];
                        let hyphen_width = self.measure("-", false);
                        
                        if prefix_width_at_start + self.measure(test_str, false) + hyphen_width <= max_width {
                            if trim_offset + current_prefix_len < word.len() {
                                best_hyphen_idx = trim_offset + current_prefix_len;
                            }
                        } else {
                            break;
                        }
                    }

                    if best_hyphen_idx > 0 {
                        let split_at = word_start + best_hyphen_idx;
                        return (&content[..split_at], content[split_at..].trim_start(), true);
                    }
                }

                if last_valid_split == 0 {
                    return (&content[..i], &content[i..], false);
                }
                return (
                    &content[..last_valid_split],
                    content[last_valid_split..].trim_start(),
                    false
                );
            }
            if c.is_whitespace() {
                last_valid_split = i;
            }
        }
        (content, "", false)
    }

    pub fn render_menu(&mut self, title: &str, items: &[&str], selected: usize) {
        self.buffer.clear();
        let max_width = WIDTH as i32 - 4;
        let start_y = self.draw_title(title, max_width);

        let max_height = HEIGHT as i32 - start_y;
        let line_height = MENU_LINE_HEIGHT;
        let max_width = WIDTH as i32 - 4;

        // Simple paging: find start of page
        let mut page_start = 0;
        let mut current_y = 0;
        for (i, item) in items.iter().enumerate() {
            let is_item_bold = item.starts_with('#');
            let h = if self.measure(item, is_item_bold) > max_width - self.menu_text_x(i == selected) {
                line_height * 2
            } else {
                line_height
            };
            if current_y + h > max_height {
                if i > selected {
                    break;
                }
                page_start = i;
                current_y = 0;
            }
            current_y += h;
        }

        let mut y_offset = 0;
        for (i, item) in items.iter().enumerate().skip(page_start) {
            if y_offset >= max_height {
                break;
            }
            y_offset += self.draw_menu_item(
                item,
                i == selected,
                start_y + y_offset,
                max_width,
                line_height,
            );
        }
    }

    pub fn render_paged_text(
        &mut self,
        title: &str,
        details: &[Detail],
        content: &str,
        page_idx: usize,
    ) -> usize {
        self.buffer.clear();

        let max_width = WIDTH as i32 - 4;

        // 1. Measure title height (simulated wrap)
        let mut title_height = 0;
        if !title.is_empty() {
            let style = Self::title_style();
            let line_height = Self::get_line_height(&style);
            let mut y = Self::get_ascent(&style);
            let mut remaining = title;
            while !remaining.is_empty() {
                let (_, rest) = self.wrap_title_text(remaining, max_width);
                remaining = rest;
                if !remaining.is_empty() {
                    y += line_height;
                }
            }
            title_height = y + TITLE_SEP_Y;
        }

        let start_y = if page_idx == 0 {
            if !title.is_empty() {
                self.draw_title(title, max_width)
            } else {
                PAGE_CONT_START_Y
            }
        } else {
            PAGE_CONT_START_Y
        };

        let page_0_offset = if title_height > 0 {
            title_height
        } else {
            PAGE_CONT_START_Y
        };
        let page_0_height = HEIGHT as i32 - page_0_offset;
        let cont_height = HEIGHT as i32 - PAGE_CONT_START_Y;
        let lh = |gy: i32| {
            if gy < page_0_height {
                BODY_LINE_HEIGHT_P0
            } else {
                BODY_LINE_HEIGHT_P1
            }
        };

        let start_y_global = if page_idx == 0 {
            0
        } else {
            page_0_height + (page_idx as i32 - 1) * cont_height
        };
        let end_y_global = start_y_global
            + (if page_idx == 0 {
                page_0_height
            } else {
                cont_height
            });

        let mut global_y = 0;
        let mut local_y = 0;

        let indent_w = self.measure("ab", false);

        // Render Details First (only on page 0 if desired, or counted towards global height)
        for detail in details {
            let label = detail.label;
            let value = detail.value;

            // Label in Bold
            if global_y >= start_y_global && global_y < end_y_global {
                let _ = Text::new(label, Point::new(2, start_y + local_y), Self::bold_style())
                    .draw(&mut self.buffer);
            }
            let label_h = lh(global_y);
            global_y += label_h;
            if global_y >= start_y_global && global_y < end_y_global {
                local_y += label_h;
            }

            // Value (potentially multiple lines, handle \n)
            for line in value.split('\n') {
                let mut remaining = line;
                while !remaining.is_empty() {
                    let x_offset = indent_w;
                    let (first, rest, has_hyphen) = self.wrap_text(remaining, max_width - x_offset);
                    if global_y >= start_y_global && global_y < end_y_global {
                        let mut disp_text: String<256> = String::new();
                        let _ = write!(disp_text, "{}{}", first, if has_hyphen { "-" } else { "" });
                        let _ = Text::new(
                            disp_text.as_str(),
                            Point::new(2 + x_offset, start_y + local_y),
                            Self::body_style(),
                        )
                        .draw(&mut self.buffer);
                    }
                    let line_h = lh(global_y);
                    global_y += line_h;
                    if global_y >= start_y_global && global_y < end_y_global {
                        local_y += line_h;
                    }
                    remaining = rest;
                }
            }
            // No extra gap after details in book style
        }

        if !details.is_empty() {
            // Tiny separator logic moved to 0 for tight style
        }

        for line in content.split('\n') {
            if line == "~" {
                if global_y >= start_y_global && global_y < end_y_global {
                    local_y += 4;
                }
                global_y += 4;
                continue;
            }
            if line.is_empty() {
                // Book style: No gap between paragraphs. Just continue.
                continue;
            }

            let is_bold = line.starts_with('#');
            let text = if is_bold {
                let skip = line.chars().next().map_or(1, |c| c.len_utf8());
                &line[skip..]
            } else {
                line
            };

            let mut remaining = text;
            let mut is_first_line = true;

            while !remaining.is_empty() {
                let current_indent = if is_first_line && !is_bold {
                    indent_w
                } else {
                    0
                };
                let (first, rest, has_hyphen) = self.wrap_text(
                    if is_bold { line } else { remaining },
                    max_width - current_indent,
                );

                if global_y >= start_y_global && global_y < end_y_global {
                    let style = if is_bold {
                        Self::bold_style()
                    } else {
                        Self::body_style()
                    };
                    let x_start = 2 + current_indent;
                    let avail = max_width - current_indent;

                    // Justification Logic
                    // Don't justify if it's the last line of the paragraph OR bold (header)
                    let is_last_line = rest.is_empty();
                    if !is_bold && !is_last_line {
                        // Gather words
                        let mut words = [ ""; 32 ];
                        let mut word_count = 0;
                        for w in first.split_whitespace() {
                            if word_count < 32 {
                                words[word_count] = w;
                                word_count += 1;
                            }
                        }

                        if word_count > 1 {
                            let mut total_word_width = 0;
                            for word in words.iter().take(word_count) {
                                total_word_width += self.measure(word, is_bold);
                            }
                            if has_hyphen {
                                total_word_width += self.measure("-", is_bold);
                            }

                            let slack = avail - total_word_width;
                            let gaps = word_count - 1;
                            let base_gap = slack / gaps as i32;
                            let mut extra_pixels = slack % gaps as i32;

                            let mut curr_x = x_start;
                            for (i, word) in words.iter().enumerate().take(word_count) {
                                let mut w_text : String<128> = String::new();
                                let _ = write!(w_text, "{}", word);
                                if i == word_count - 1 && has_hyphen {
                                    let _ = write!(w_text, "-");
                                }
                                
                                let _ = Text::new(w_text.as_str(), Point::new(curr_x, start_y + local_y), style.clone())
                                    .draw(&mut self.buffer);
                                
                                curr_x += self.measure(w_text.as_str(), is_bold);
                                if i < gaps {
                                    curr_x += base_gap;
                                    if extra_pixels > 0 {
                                        curr_x += 1;
                                        extra_pixels -= 1;
                                    }
                                }
                            }
                        } else {
                            // Single word line, Fallback to left align
                             let mut disp: String<256> = String::new();
                             let _ = write!(disp, "{}{}", first, if has_hyphen { "-" } else { "" });
                             let _ = Text::new(disp.as_str(), Point::new(x_start, start_y + local_y), style.clone())
                                .draw(&mut self.buffer);
                        }
                    } else {
                        // Standard Left-Align (headers or last line)
                        let mut disp: String<256> = String::new();
                        let _ = write!(disp, "{}{}", first, if has_hyphen { "-" } else { "" });
                        let _ = Text::new(disp.as_str(), Point::new(x_start, start_y + local_y), style.clone())
                            .draw(&mut self.buffer);
                    }

                    local_y += lh(global_y);
                }
                global_y += lh(global_y);
                remaining = rest;
                is_first_line = false;

                if first.is_empty() && !remaining.is_empty() {
                    let skip = remaining.chars().next().map_or(1, |c| c.len_utf8());
                    remaining = &remaining[skip..];
                }
            }
        }

        if global_y <= page_0_height {
            1
        } else {
            (1 + (global_y - page_0_height + cont_height - 1) / cont_height) as usize
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_wrap_text_hyphen() {
        let ui = Ui::new();
        let text = "unbelievable";
        let max_width = 70; 
        let (first, rest, has_hyphen) = ui.wrap_text(text, max_width);
        assert!(has_hyphen, "Expected hyphenation for 'unbelievable'");
        assert!(!first.is_empty());
        assert!(!rest.is_empty());
    }

    #[test]
    fn test_wrap_text_simple() {
        let ui = Ui::new();
        let text = "A short story.";
        let (first, rest, has_hyphen) = ui.wrap_text(text, 120);
        assert!(!has_hyphen);
        assert_eq!(first, "A short story.");
        assert_eq!(rest, "");
    }

    #[test]
    fn test_display_buffer_new() {
        let buf = DisplayBuffer::new();
        assert_eq!(buf.buffer[0], 0xFF);
    }
}
