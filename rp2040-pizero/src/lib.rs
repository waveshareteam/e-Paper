#![no_std]

pub mod config;
#[cfg(all(target_arch = "arm", target_os = "none"))]
pub mod epd;
pub mod gui;
