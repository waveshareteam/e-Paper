/*
 Example guide: https://www.amebaiot.com/en/amebapro2-arduino-epaper-text/
 
 */

#include "image.h"
#include "epdif_2in9.h"
#include "epdpaint.h"

// SPI Pin definition
// Any GPIO pins can be used for EPD_BUSY_Pin & EPD_RES_Pin, you may refer to the pinmap of the board you are using
#define EPD_BUSY_Pin        4
#define EPD_RES_Pin         5
#define EPD_SPI_CS_Pin      SPI_SS
#define EPD_SPI_MOSI_Pin    SPI_MOSI
#define EPD_SPI_MISO_Pin    SPI_MISO
#define EPD_SPI_CLK_Pin     SPI_SCLK

#define COLOR_BLACK        0
#define COLOR_WHITE        1

EpdIf2in9 epd2in9(EPD_BUSY_Pin, EPD_RES_Pin, EPD_SPI_CS_Pin, EPD_SPI_MOSI_Pin, EPD_SPI_MISO_Pin, EPD_SPI_CLK_Pin);
unsigned char image[1024];
Paint paint(image, 0, 0);     // width should be the multiple of 8

void setup() {
    pinMode(EPD_BUSY_Pin, INPUT);       // EPD_BUSY_Pin
    pinMode(EPD_RES_Pin, OUTPUT);       // EPD_RES_Pin
    pinMode(EPD_SPI_CS_Pin, OUTPUT);    // EPD_SPI_CS_Pin
    pinMode(EPD_SPI_MOSI_Pin, OUTPUT);  // EPD_SPI_MOSI_Pin
    pinMode(EPD_SPI_MISO_Pin, OUTPUT);  // EPD_SPI_MISO_Pin
    pinMode(EPD_SPI_CLK_Pin, OUTPUT);   // EPD_SPI_CLK_Pin
}

void loop() {
    DisplayText();
    epd2in9.EPD_Sleep();
}

void DisplayText(void) {
    epd2in9.EPD_SetFrameMemory_Base(gImage_basemapNA);

    paint.SetRotate(ROTATE_0);
    paint.SetWidth(128);
    paint.SetHeight(24);

    paint.Clear(COLOR_BLACK);
    paint.DrawStringAt(0, 4, "Hello Ameba", &Font16, COLOR_WHITE);
    epd2in9.EPD_SetFrameMemory(paint.GetImage(), 0, 10, paint.GetWidth(), paint.GetHeight());

    paint.Clear(COLOR_WHITE);
    paint.DrawStringAt(0, 4, "1234567890", &Font16, COLOR_BLACK);
    epd2in9.EPD_SetFrameMemory(paint.GetImage(), 0, 40, paint.GetWidth(), paint.GetHeight());

    paint.Clear(COLOR_BLACK);
    paint.DrawStringAt(0, 4, "+_*/><:*&^%", &Font16, COLOR_WHITE);
    epd2in9.EPD_SetFrameMemory(paint.GetImage(), 0, 70, paint.GetWidth(), paint.GetHeight());

    epd2in9.EPD_DisplayFrame();
}
