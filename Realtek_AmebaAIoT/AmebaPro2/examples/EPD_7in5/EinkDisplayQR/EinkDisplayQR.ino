/*
 Example guide: https://www.amebaiot.com/en/amebapro2-arduino-epaper-qrcode/

 */

#include "epdif_7in5.h"
#include "qrcode.h"
#include "epdpaint.h"

// SPI Pin definition
// Any GPIO pins can be used for BUSY_PIN, RST_PIN, and DC_PIN. You may refer to the pinmap of the board that you are using.
#define BUSY_PIN        4
#define RST_PIN         5
#define DC_PIN          6
#define CS_PIN          SPI_SS

#define QRCODE_VERSION    5

EpdIf7in5 epd7in5(BUSY_PIN, RST_PIN, DC_PIN, CS_PIN);

unsigned char image[48000]; 
Paint paint(image, 800, 480);    //width should be the multiple of 8

// Create the QR code
QRCode qrcode;

void setup() {
    Serial.begin(115200);
    Serial.print("e-Paper init \r\n ");

    if (epd7in5.Init() != 0) {
        Serial.print("e-Paper init failed");
        return;
    }
}

void loop() {
    PrintQRCode("https://www.amebaiot.com/en/ameba-arduino-summary/");
    epd7in5.Sleep();
}

void PrintQRCode(const char *url) {
    paint.Clear(COLOR_WHITE);
    epd7in5.DisplayFrame(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());

    const int ps = 6; // pixels / square : used to define the size of QR
    uint8_t qrcodeData[qrcode_getBufferSize(QRCODE_VERSION)];
    qrcode_initText(&qrcode, qrcodeData, QRCODE_VERSION, ECC_LOW, url);

    paint.SetRotate(ROTATE_0);
    paint.SetWidth (400);
    paint.SetHeight(240);
    for (uint8_t y = 0; y < qrcode.size; y++) {
        for (uint8_t x = 0; x < qrcode.size; x++) {
            if (qrcode_getModule(&qrcode, x, y)) {
                for (int xi = (x * ps + 2); xi < (x * ps + ps + 2); xi++) { // If pixel is on, we draw a "ps x ps" black square
                    for (int yi = (y * ps + 2); yi < (y * ps + ps + 2); yi++) {
                        paint.DrawPixel(xi, yi, COLOR_BLACK);
                    }
                }
            }
        }
    }
    epd7in5.DisplayFrame(paint.GetImage(), 280, 130, paint.GetWidth(), paint.GetHeight());
}
