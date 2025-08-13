/*
 Example guide: https://www.amebaiot.com/en/amebapro2-arduino-epaper-qrcode/

 */

#include "epdif_2in9b.h"
#include "image.h"
#include "epdpaint.h"
#include "qrcode.h"

// SPI Pin definition
// Any GPIO pins can be used for BUSY_PIN, RST_PIN, and DC_PIN. You may refer to the pinmap of the board that you are using.
#define BUSY_PIN        4
#define RST_PIN         5
#define DC_PIN          6
#define CS_PIN          SPI_SS

#define COLORED     0
#define UNCOLORED   1
#define QRCODE_VERSION    5

EpdIf2in9b epd2in9b(BUSY_PIN, RST_PIN, DC_PIN, CS_PIN);
unsigned char image[1024];
Paint paint(image, 0, 0);     // width should be the multiple of 8

// Create the QR code
QRCode qrcode;

void setup() {
    Serial.begin(115200);
    Serial.print("e-Paper init \r\n ");

    if (epd2in9b.Init() != 0) {
        Serial.print("e-Paper init failed");
        return;
    }
}

void loop() {
    epd2in9b.ClearFrame();
    PrintQRCode("https://www.amebaiot.com/en/ameba-arduino-summary/");
    epd2in9b.Sleep();
}

void PrintQRCode(const char * url) {
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
   
    epd2in9b.ClearFrame();
    epd2in9b.SetPartialWindowBlack(paint.GetImage(), 30, 100, paint.GetWidth(), paint.GetHeight());
    epd2in9b.DisplayFrame();
}
