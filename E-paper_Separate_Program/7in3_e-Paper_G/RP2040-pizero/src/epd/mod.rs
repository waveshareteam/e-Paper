pub mod epd_7in3g;
pub use epd_7in3g::*;

#[allow(async_fn_in_trait)]
#[allow(dead_code)]
pub trait AsyncEpd {
    type Error;
    async fn init(&mut self) -> Result<(), Self::Error>;
    async fn clear(&mut self) -> Result<(), Self::Error>;
    async fn sleep(&mut self) -> Result<(), Self::Error>;
    async fn display_base(&mut self, buffer: &[u8]) -> Result<(), Self::Error>;
}
