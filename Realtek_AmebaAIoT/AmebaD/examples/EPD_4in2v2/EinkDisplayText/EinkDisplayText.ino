#include "epdif_4in2_v2.h"
#include "epdpaint.h"

// SPI Pin definition
// Any GPIO pins can be used for BUSY_PIN, RST_PIN, and DC_PIN. You may refer to the pinmap of the board that you are using.
#define BUSY_PIN        8
#define RST_PIN         9
#define DC_PIN          6
#define CS_PIN          SPI_SS

EpdIf4in2V2 epd4in2v2(BUSY_PIN, RST_PIN, DC_PIN, CS_PIN);

void setup() {
    Serial.begin(9600);
    Serial.print("e-Paper init \r\n ");
    
    if (epd4in2v2.Init() != 0) {
    Serial.print("e-Paper init failed");
    return;
  }

  epd4in2v2.Clear();
}

void loop() {
    DisplayText();
    epd4in2v2.Sleep();
}

void DisplayText(void) {

    unsigned char image[1500];
    Paint paint(image, 400, 28);    //width should be the multiple of 8 

    paint.Clear(COLOR_BLACK);
    paint.DrawStringAt(20, 5, "Hello Ameba", &Font24, COLOR_WHITE);
    epd4in2v2.Display_Partial_Not_refresh(paint.GetImage(), 0, 32, 0+paint.GetWidth(), 32+paint.GetHeight());

    paint.Clear(COLOR_WHITE);
    paint.DrawStringAt(20, 5, "1234567890", &Font24, COLOR_BLACK);
    epd4in2v2.Display_Partial_Not_refresh(paint.GetImage(), 0, 64, 0+paint.GetWidth(), 64+paint.GetHeight());

    paint.Clear(COLOR_BLACK);
    paint.DrawStringAt(20, 5, "+_*/><:*&^%", &Font24, COLOR_WHITE);
    epd4in2v2.Display_Partial(paint.GetImage(), 0, 96, 0+paint.GetWidth(), 96+paint.GetHeight());
}
