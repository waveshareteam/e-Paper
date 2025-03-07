#include <SPI.h>
#include "epdif_2in9b.h"
#include "epdpaint.h"
#include "image.h"

// SPI Pin definition
// Any GPIO pins can be used for BUSY_PIN, RST_PIN, and DC_PIN. You may refer to the pinmap of the board that you are using.
#define BUSY_PIN        8
#define RST_PIN         9
#define DC_PIN          6
#define CS_PIN          SPI_SS

#define COLORED     0
#define UNCOLORED   1

EpdIf2in9b epd2in9b(BUSY_PIN, RST_PIN, DC_PIN, CS_PIN);

unsigned char image[1024];
Paint paint(image, 0, 0);     // width should be the multiple of 8

void setup() {
    Serial.begin(115200);
    Serial.print("e-Paper init \r\n ");

    if (epd2in9b.Init() != 0) {
      Serial.print("e-Paper init failed");
      return;
    }
}

void loop() {
    DisplayText();
    epd2in9b.Sleep();
}

void DisplayText(void) {
    epd2in9b.ClearFrame();

    paint.SetWidth(128);
    paint.SetHeight(24);

    paint.Clear(UNCOLORED);
    paint.DrawStringAt(0, 4, "Hello Ameba", &Font16, COLORED);
    epd2in9b.SetPartialWindowBlack(paint.GetImage(), 0, 10, paint.GetWidth(), paint.GetHeight());

    paint.Clear(COLORED);
    paint.DrawStringAt(0, 4, "1234567890", &Font16, UNCOLORED);
    epd2in9b.SetPartialWindowRed(paint.GetImage(), 0, 40, paint.GetWidth(), paint.GetHeight());

    paint.Clear(COLORED);
    paint.DrawStringAt(0, 4, "+_*/><:*&^%", &Font16, COLOR_WHITE);
    epd2in9b.SetPartialWindowBlack(paint.GetImage(), 0, 70, paint.GetWidth(), paint.GetHeight());

    epd2in9b.DisplayFrame();
}
