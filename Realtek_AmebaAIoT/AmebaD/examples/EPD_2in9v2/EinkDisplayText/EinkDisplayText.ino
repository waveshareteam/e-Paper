#include "epdif_2in9_v2.h"
#include "epdpaint.h"
#include "image.h"

// SPI Pin definition
// Any GPIO pins can be used for BUSY_PIN, RST_PIN, and DC_PIN. You may refer to the pinmap of the board that you are using.
#define BUSY_PIN        8
#define RST_PIN         9
#define DC_PIN          6
#define CS_PIN          SPI_SS

EpdIf2in9V2 epd2in9v2(BUSY_PIN, RST_PIN, DC_PIN, CS_PIN);

bool textDisplay = false;

unsigned char image[1024];
Paint paint(image, 0, 0);     // width should be the multiple of 8

void setup() {
    Serial.begin(115200);
    Serial.print("e-Paper init \r\n ");
    
    if (epd2in9v2.Init() != 0) {
      Serial.print("e-Paper init failed");
      return;
    }

    epd2in9v2.SetFrameMemory_Base(gImage_basemapNA);
}

void loop() {
  if (!textDisplay) {
    DisplayText();
    textDisplay = true;
  } 
  epd2in9v2.Sleep();
}

void DisplayText(void) {

    paint.SetRotate(ROTATE_0);
    paint.SetWidth(128);
    paint.SetHeight(24);

    paint.Clear(COLOR_BLACK);
    paint.DrawStringAt(0, 4, "Hello Ameba", &Font16, COLOR_WHITE);
    epd2in9v2.SetFrameMemory(paint.GetImage(), 0, 10, paint.GetWidth(), paint.GetHeight());

    paint.Clear(COLOR_WHITE);
    paint.DrawStringAt(0, 4, "1234567890", &Font16, COLOR_BLACK);
    epd2in9v2.SetFrameMemory(paint.GetImage(), 0, 40, paint.GetWidth(), paint.GetHeight());

    paint.Clear(COLOR_BLACK);
    paint.DrawStringAt(0, 4, "+_*/><:*&^%", &Font16, COLOR_WHITE);
    epd2in9v2.SetFrameMemory(paint.GetImage(), 0, 70, paint.GetWidth(), paint.GetHeight());

    epd2in9v2.DisplayFrame();
}
