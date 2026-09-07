/*
 Example guide: https://www.amebaiot.com/en/amebapro2-arduino-epaper/

 */

#include "epdif_2in9.h"
#include "image.h"

// SPI Pin definition
// Any GPIO pins can be used for EPD_BUSY_Pin & EPD_RES_Pin, you may refer to the pinmap of the board you are using
#define EPD_BUSY_Pin        4
#define EPD_RES_Pin         5
#define EPD_SPI_CS_Pin      SPI_SS
#define EPD_SPI_MOSI_Pin    SPI_MOSI
#define EPD_SPI_MISO_Pin    SPI_MISO
#define EPD_SPI_CLK_Pin     SPI_SCLK

EpdIf2in9 epd2in9(EPD_BUSY_Pin, EPD_RES_Pin, EPD_SPI_CS_Pin, EPD_SPI_MOSI_Pin, EPD_SPI_MISO_Pin, EPD_SPI_CLK_Pin);

void setup() {
    pinMode(EPD_BUSY_Pin, INPUT);       // EPD_BUSY_Pin
    pinMode(EPD_RES_Pin, OUTPUT);       // EPD_RES_Pin
    pinMode(EPD_SPI_CS_Pin, OUTPUT);    // EPD_SPI_CS_Pin
    pinMode(EPD_SPI_MOSI_Pin, OUTPUT);  // EPD_SPI_MOSI_Pin
    pinMode(EPD_SPI_MISO_Pin, OUTPUT);  // EPD_SPI_MISO_Pin
    pinMode(EPD_SPI_CLK_Pin, OUTPUT);   // EPD_SPI_CLK_Pin
}

void loop() {
    DisplayImage();   // Display an image
    DisplayQRCode();  // Displays QR codes every 4-5 sec
    DisplayGIF();     // Display a 3 frame GIF
}

void DisplayImage(void) {
    /* User predefined image files "gImage_basemapIMG"  (296 x 128)
     can be modified in "image.h"
    */
    epd2in9.EPD_SetFrameMemory_Base(gImage_basemapIMG);   
    delay(4000);
}

void DisplayQRCode(void) {
    epd2in9.EPD_SetFrameMemory_Base(gImage_QR0);          // fb website 296 x 128
    delay(4000);
    epd2in9.EPD_SetFrameMemory_Base(gImage_QR1);          // official website 296 x 128
    delay(4000);
    epd2in9.EPD_SetFrameMemory_Base(gImage_QR2);          // communicty website 296 x 128
    delay(4000);
}

void DisplayGIF(void) {
    epd2in9.EPD_SetFrameMemory_Base(gImage_basemapNA);
    delay(100);
    for (int i = 0; i < 5; i++) {
        epd2in9.EPD_SetFrameMemory_Partial(20, 200, gImage_f1, 128, 64);  // x,y,DATA,resolution 128 x 64
        epd2in9.EPD_DisplayFrame();
        epd2in9.EPD_SetFrameMemory_Partial(20, 200, gImage_f2, 128, 64);  // x,y,DATA,resolution 128 x 64
        epd2in9.EPD_DisplayFrame();
        epd2in9.EPD_SetFrameMemory_Partial(20, 200, gImage_f3, 128, 64);  // x,y,DATA,resolution 128 x 64
        epd2in9.EPD_DisplayFrame();
    }
}
