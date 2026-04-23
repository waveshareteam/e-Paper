#![cfg_attr(not(test), no_std)]

pub mod data;
#[cfg(all(target_arch = "arm", target_os = "none"))]
pub mod epd;
pub mod ui;
pub mod app_logic;
pub mod assets;
