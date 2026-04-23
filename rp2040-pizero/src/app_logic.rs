#[derive(Clone, Copy, PartialEq, Debug)]
pub enum AppState {
    CoverImage,
    SelectingCategory,
    SelectingItem,
    ViewingItem { page: usize },
    ViewingCategoryDescription { page: usize },
}

#[derive(Clone, Copy, PartialEq, Debug)]
pub enum UiEvent {
    Up,
    Down,
    Select,
    Back,
    LongBack,
    ForceRefresh,
}

pub struct BtnState {
    pub press_duration: u16,
    pub handled: bool,
}

impl Default for BtnState {
    fn default() -> Self {
        Self::new()
    }
}

impl BtnState {
    pub const fn new() -> Self {
        Self {
            press_duration: 0,
            handled: false,
        }
    }

    pub fn tick(&mut self, is_pressed: bool, repeat_delay: u16, long_press: u16) -> Option<UiEvent> {
        if is_pressed {
            self.press_duration += 1;
            if self.press_duration == 1 {
                // Initial click (handled by caller for some buttons)
                None
            } else if self.press_duration == long_press && !self.handled {
                self.handled = true;
                Some(UiEvent::LongBack)
            } else if self.press_duration >= repeat_delay {
                // Repeat logic
                Some(UiEvent::Down) // This is a bit specific, might need refinement
            } else {
                None
            }
        } else {
            let event = if self.press_duration > 1 && !self.handled {
                // Short press release
                Some(UiEvent::Select) // Again, specific
            } else {
                None
            };
            self.press_duration = 0;
            self.handled = false;
            event
        }
    }
}

// Better approach: move the state transition logic here
impl AppState {
    pub fn next_state(self, event: UiEvent, category_idx: &mut usize, item_idx: &mut usize, total_categories: usize, total_items: usize, total_pages: usize) -> Option<Self> {
        match (self, event) {
            (AppState::CoverImage, UiEvent::Select | UiEvent::Up | UiEvent::Down | UiEvent::Back) => {
                Some(AppState::SelectingCategory)
            }
            
            (AppState::SelectingCategory, UiEvent::Up) => {
                *category_idx = if *category_idx == 0 { total_categories - 1 } else { *category_idx - 1 };
                None
            }
            (AppState::SelectingCategory, UiEvent::Down) => {
                *category_idx = if *category_idx + 1 >= total_categories { 0 } else { *category_idx + 1 };
                None
            }
            (AppState::SelectingCategory, UiEvent::Select) => {
                *item_idx = 0;
                Some(AppState::SelectingItem)
            }
            
            (AppState::SelectingItem, UiEvent::Up) => {
                *item_idx = if *item_idx == 0 { total_items - 1 } else { *item_idx - 1 };
                None
            }
            (AppState::SelectingItem, UiEvent::Down) => {
                *item_idx = if *item_idx + 1 >= total_items { 0 } else { *item_idx + 1 };
                None
            }
            (AppState::SelectingItem, UiEvent::Select) => {
                if *item_idx == 0 {
                    Some(AppState::ViewingCategoryDescription { page: 0 })
                } else {
                    Some(AppState::ViewingItem { page: 0 })
                }
            }
            (AppState::SelectingItem, UiEvent::Back) => Some(AppState::SelectingCategory),

            (AppState::ViewingItem { page }, UiEvent::Up) if page > 0 => {
                Some(AppState::ViewingItem { page: page - 1 })
            }
            (AppState::ViewingCategoryDescription { page }, UiEvent::Up) if page > 0 => {
                Some(AppState::ViewingCategoryDescription { page: page - 1 })
            }

            (AppState::ViewingItem { page }, UiEvent::Down) if page + 1 < total_pages => {
                Some(AppState::ViewingItem { page: page + 1 })
            }
            (AppState::ViewingCategoryDescription { page }, UiEvent::Down) if page + 1 < total_pages => {
                Some(AppState::ViewingCategoryDescription { page: page + 1 })
            }

            (AppState::ViewingItem { .. } | AppState::ViewingCategoryDescription { .. }, UiEvent::Back) => {
                Some(AppState::SelectingItem)
            }

            (_, UiEvent::ForceRefresh) => Some(self),

            // Universal escape
            (_, UiEvent::LongBack) => Some(AppState::CoverImage),

            _ => None,
        }
    }
}
