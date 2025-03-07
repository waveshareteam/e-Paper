/*

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-epaper-qrocde/
 */

#include <avr/pgmspace.h>

#include "epdif.h"
#include "epdpaint.h"
#include "image.h"
#include "qrcode.h"

// SPI Pin definition
// Any GPIO pins can be used for EPD_BUSY_Pin & EPD_RES_Pin, you may refer to the pinmap of the board you are using
#define EPD_BUSY_Pin        8
#define EPD_RES_Pin         9
#define EPD_SPI_CS_Pin      SPI_SS
#define EPD_SPI_MOSI_Pin    SPI_MOSI
#define EPD_SPI_MISO_Pin    SPI_MISO
#define EPD_SPI_CLK_Pin     SPI_SCLK

#define COLORED           0
#define UNCOLORED         1
#define QRCODE_VERSION    5

EpdIf EPD(EPD_BUSY_Pin, EPD_RES_Pin, EPD_SPI_CS_Pin, EPD_SPI_MOSI_Pin, EPD_SPI_MISO_Pin, EPD_SPI_CLK_Pin);
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
    EPD.EPD_SetRAMValue_BaseMap(gImage_basemapNA);
    PrintQRCode("https://www.amebaiot.com/en/ameba-arduino-summary/");
    EPD.EPD_Sleep();
}

void PrintQRCode(const char * url){
    QRCode qrcode;
    const int ps = 2;               // pixels / square : used to define the size of QR
    uint8_t qrcodeData[qrcode_getBufferSize(QRCODE_VERSION)];
    qrcode_initText(&qrcode, qrcodeData, QRCODE_VERSION, ECC_LOW, url);

    paint.SetRotate(ROTATE_0);
    paint.SetWidth (80);
    paint.SetHeight(80);
    paint.Clear(UNCOLORED);

    for (uint8_t y = 0; y < qrcode.size; y++) {
        for (uint8_t x = 0; x < qrcode.size; x++) {
            if (qrcode_getModule(&qrcode, x, y)) {
                for (int xi = (x * ps + 2); xi < (x * ps + ps + 2); xi++) { // If pixel is on, we draw a "ps x ps" black square
                    for (int yi = (y * ps + 2); yi < (y * ps + ps + 2); yi++) {
                        paint.DrawPixel(xi, yi, COLORED);
                    }
                }
            }
        }
    }
    EPD.EPD_ClearScreen_White();  // Display whole screen in white
    EPD.EPD_Dis_Part(30, 200, paint.GetImage(), paint.GetWidth(), paint.GetHeight());
    EPD.EPD_UpdateDisplay();
}
