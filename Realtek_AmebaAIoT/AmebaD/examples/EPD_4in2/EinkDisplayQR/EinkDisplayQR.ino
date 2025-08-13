#include "epdif_4in2.h"
#include "qrcode.h"
#include "epdpaint.h"

// SPI Pin definition
// Any GPIO pins can be used for BUSY_PIN, RST_PIN, and DC_PIN. You may refer to the pinmap of the board that you are using.
#define BUSY_PIN        8
#define RST_PIN         9
#define DC_PIN          6
#define CS_PIN          SPI_SS

#define QRCODE_VERSION  5

EpdIf4in2 epd4in2(BUSY_PIN, RST_PIN, DC_PIN, CS_PIN);

unsigned char image[15000];
Paint paint(image, 400, 300);    //width should be the multiple of 8

// Create the QR code
QRCode qrcode;

void setup() {
    Serial.begin(115200);
    Serial.print("e-Paper init \r\n ");

    if (epd4in2.Init() != 0) {
        Serial.print("e-Paper init failed");
        return;
    }
}

void loop() {
    PrintQRCode("https://www.amebaiot.com/en/ameba-arduino-summary/");
    epd4in2.Sleep();
}

void PrintQRCode(const char *url) {
    paint.Clear(COLOR_WHITE);
    epd4in2.SetFrame_Partial(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
    
    const int ps = 4; // pixels / square : used to define the size of QR
    uint8_t qrcodeData[qrcode_getBufferSize(QRCODE_VERSION)];
    qrcode_initText(&qrcode, qrcodeData, QRCODE_VERSION, ECC_LOW, url);

    paint.SetRotate(ROTATE_0);
    paint.SetWidth (200);
    paint.SetHeight(200);
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
    epd4in2.SetFrame_Partial(paint.GetImage(), 120, 80, paint.GetWidth(), paint.GetHeight());
    epd4in2.DisplayFrame();
}
