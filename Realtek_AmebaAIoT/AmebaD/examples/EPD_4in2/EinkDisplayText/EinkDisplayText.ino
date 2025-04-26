#include "epdif_4in2.h"
#include "epdpaint.h"

// SPI Pin definition
// Any GPIO pins can be used for BUSY_PIN, RST_PIN, and DC_PIN. You may refer to the pinmap of the board that you are using.
#define BUSY_PIN        8
#define RST_PIN         9
#define DC_PIN          6
#define CS_PIN          SPI_SS

EpdIf4in2 epd4in2(BUSY_PIN, RST_PIN, DC_PIN, CS_PIN);

unsigned char image[15000];
Paint paint(image, 400, 300);    //width should be the multiple of 8

void setup() {
    Serial.begin(115200);
    Serial.print("e-Paper init \r\n ");
    
    if (epd4in2.Init() != 0) {
      Serial.print("e-Paper init failed");
      return;
    }
}

void loop() {
    DisplayText();
    epd4in2.Sleep();
}

void DisplayText(void) {
    paint.Clear(COLOR_WHITE);
    epd4in2.SetFrame_Partial(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
    
    paint.SetWidth(400);
    paint.SetHeight(40);
    
    paint.Clear(COLOR_BLACK);
    paint.DrawStringAt(0, 0, "Hello Ameba", &Font24, COLOR_WHITE);
    epd4in2.SetFrame_Partial(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());

    paint.Clear(COLOR_WHITE);
    paint.DrawStringAt(0, 0, "1234567890", &Font24, COLOR_BLACK);
    epd4in2.SetFrame_Partial(paint.GetImage(), 0, 40, paint.GetWidth(), paint.GetHeight());

    paint.Clear(COLOR_BLACK);
    paint.DrawStringAt(0, 0, "+_*/><:*&^%", &Font24, COLOR_WHITE);
    epd4in2.SetFrame_Partial(paint.GetImage(), 0, 80, paint.GetWidth(), paint.GetHeight());

    // /* This displays the data from the SRAM in e-Paper module */
    epd4in2.DisplayFrame();
}
