/*
 Example guide: https://www.amebaiot.com/en/amebapro2-arduino-epaper/

 */

#include "epdif_7in5.h"
#include "image.h"

// SPI Pin definition
// Any GPIO pins can be used for BUSY_PIN, RST_PIN, and DC_PIN. You may refer to the pinmap of the board that you are using.
#define BUSY_PIN        4
#define RST_PIN         5
#define DC_PIN          6
#define CS_PIN          SPI_SS

EpdIf7in5 epd7in5(BUSY_PIN, RST_PIN, DC_PIN, CS_PIN);

void setup() {
    Serial.begin(115200);
    Serial.print("e-Paper init \r\n ");

    if (epd7in5.Init() != 0) {
      Serial.print("e-Paper init failed");
      return;
    }
}

void loop() {
    DisplayImage();
    DisplayQRCode();
}

void DisplayImage(void) {
    epd7in5.DisplayFrame(gImage_IMG_7_5);
    delay(4000);
}

void DisplayQRCode(void) {
    epd7in5.DisplayFrame(gImage_AmebaIoT_QR_7_5);
    delay(4000);
    epd7in5.DisplayFrame(gImage_Facebook_QR_7_5);
    delay(4000);
    epd7in5.DisplayFrame(gImage_AmebaIoT_Forum_QR_7_5);
    delay(4000);
}