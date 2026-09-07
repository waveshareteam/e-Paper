#include "epdif_4in2_v2.h"
#include "image.h"

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
    DisplayImage();
    DisplayQRCode();
}

void DisplayImage(void) {
    epd4in2v2.Display(gImage_IMG_4_2);
    delay(4000);
}

void DisplayQRCode(void) {
    epd4in2v2.Display(gImage_AmebaIoT_QR_4_2);
    delay(4000);
    epd4in2v2.Display(gImage_Facebook_QR_4_2);
    delay(4000);
    epd4in2v2.Display(gImage_AmebaIoT_Forum_QR_4_2);
    delay(4000);
}
