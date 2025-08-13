#include "epdif_7in5.h"
#include "epdpaint.h"

// SPI Pin definition
// Any GPIO pins can be used for BUSY_PIN, RST_PIN, and DC_PIN. You may refer to the pinmap of the board that you are using.
#define BUSY_PIN        8
#define RST_PIN         9
#define DC_PIN          6
#define CS_PIN          SPI_SS

EpdIf7in5 epd7in5(BUSY_PIN, RST_PIN, DC_PIN, CS_PIN);

unsigned char image[48000];
Paint paint(image, 800, 480);    //width should be the multiple of 8

void setup() {
    Serial.begin(115200);
    Serial.print("e-Paper init \r\n ");

    if (epd7in5.Init() != 0) {
        Serial.print("e-Paper init failed");
        return;
    }
}

void loop() {
    DisplayText();
    epd7in5.Sleep();
}

void DisplayText(void) {
    paint.Clear(COLOR_WHITE);
    paint.SetRotate(ROTATE_90);

    paint.DrawFilledRectangle(0, 0, 480, 50, COLOR_BLACK);
    paint.DrawStringAt(0, 0, "Hello Ameba", &Font24, COLOR_WHITE);

    paint.DrawFilledRectangle(0, 50, 480, 100, COLOR_WHITE);
    paint.DrawStringAt(0, 60, "1234567890", &Font24, COLOR_BLACK);

    paint.DrawFilledRectangle(0, 100, 480, 150, COLOR_BLACK);
    paint.DrawStringAt(0, 120, "+_*/><:*&^%", &Font24, COLOR_WHITE);

    epd7in5.DisplayFrame(paint.GetImage());
}
