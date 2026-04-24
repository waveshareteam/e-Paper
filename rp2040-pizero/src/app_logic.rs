#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum AppState {
    CoverImage,
    SelectingCategory,
    SelectingItem,
    ViewingItem { page: usize },
    ViewingCategoryDescription { page: usize },
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum UiEvent {
    Up,
    Down,
    Select,
    Back,
    LongBack,
    ForceRefresh,
}

#[derive(Debug, Default)]
pub struct BtnState {
    pub press_duration: u16,
    pub handled: bool,
}

impl BtnState {
    pub const fn new() -> Self {
        Self {
            press_duration: 0,
            handled: false,
        }
    }
}

impl AppState {
    pub fn next_state(
        self,
        event: UiEvent,
        category_idx: &mut usize,
        item_idx: &mut usize,
        total_categories: usize,
        total_items: usize,
        total_pages: usize,
    ) -> Option<Self> {
        match (self, event) {
            (AppState::CoverImage, UiEvent::Select | UiEvent::Up | UiEvent::Down | UiEvent::Back) => {
                Some(AppState::SelectingCategory)
            }

            (AppState::SelectingCategory, UiEvent::Up) => {
                *category_idx = category_idx.checked_sub(1).unwrap_or(total_categories - 1);
                None
            }
            (AppState::SelectingCategory, UiEvent::Down) => {
                *category_idx = (*category_idx + 1) % total_categories;
                None
            }
            (AppState::SelectingCategory, UiEvent::Select) => {
                *item_idx = 0;
                Some(AppState::SelectingItem)
            }

            (AppState::SelectingItem, UiEvent::Up) => {
                *item_idx = item_idx.checked_sub(1).unwrap_or(total_items - 1);
                None
            }
            (AppState::SelectingItem, UiEvent::Down) => {
                *item_idx = (*item_idx + 1) % total_items;
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

            (AppState::ViewingItem { page }, UiEvent::Up) => {
                page.checked_sub(1).map(|p| AppState::ViewingItem { page: p })
            }
            (AppState::ViewingCategoryDescription { page }, UiEvent::Up) => {
                page.checked_sub(1).map(|p| AppState::ViewingCategoryDescription { page: p })
            }

            (AppState::ViewingItem { page }, UiEvent::Down) => {
                (page + 1 < total_pages).then_some(AppState::ViewingItem { page: page + 1 })
            }
            (AppState::ViewingCategoryDescription { page }, UiEvent::Down) => {
                (page + 1 < total_pages).then_some(AppState::ViewingCategoryDescription { page: page + 1 })
            }

            (AppState::ViewingItem { .. } | AppState::ViewingCategoryDescription { .. }, UiEvent::Back) => {
                Some(AppState::SelectingItem)
            }

            (_, UiEvent::ForceRefresh) => Some(self),
            (_, UiEvent::LongBack) => Some(AppState::CoverImage),
            _ => None,
        }
    }
}
