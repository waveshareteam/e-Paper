/*
 Example guide: https://www.amebaiot.com/en/amebapro2-arduino-epaper/

 */

#include "epdif_2in9_v2.h"
#include "image.h"

// SPI Pin definition
// Any GPIO pins can be used for BUSY_PIN, RST_PIN, and DC_PIN. You may refer to the pinmap of the board that you are using.
#define BUSY_PIN        4
#define RST_PIN         5
#define DC_PIN          6
#define CS_PIN          SPI_SS

EpdIf2in9V2 epd2in9v2(BUSY_PIN, RST_PIN, DC_PIN, CS_PIN);

void setup() {
    Serial.begin(115200);
    Serial.print("e-Paper init \r\n ");
    if (epd2in9v2.Init() != 0) {
        Serial.print("e-Paper init failed");
        return;
    }
}

void loop() {
    DisplayImage();
    DisplayQRCode();
    DisplayGIF();
}

void DisplayImage(void) {
    /* User predefined image files "gImage_basemapIMG"  (296 x 128)
    can be modified in "image.h"
    */
    epd2in9v2.SetFrameMemory_Base(gImage_basemapIMG);
    epd2in9v2.DisplayFrame();
    delay(4000);
}

void DisplayQRCode(void) {
    epd2in9v2.SetFrameMemory_Base(gImage_Realtek_QR); // fb website 296 x 128
    epd2in9v2.DisplayFrame();
    delay(4000);
    epd2in9v2.SetFrameMemory_Base(gImage_Facebook_QR); // official website 296 x 128
    epd2in9v2.DisplayFrame();
    delay(4000);
    epd2in9v2.SetFrameMemory_Base(gImage_AmebaIoT_QR); // community website 296 x 128
    epd2in9v2.DisplayFrame();
    delay(4000);
}

void DisplayGIF(void) {
    epd2in9v2.SetFrameMemory_Base(gImage_basemapNA);
    epd2in9v2.DisplayFrame();
    delay(100);

    for (int i = 0; i < 5; i++) {
      epd2in9v2.SetFrameMemory_Partial(gImage_IMG1, 0, 40, 128, 296);  // x,y,DATA,resolution 128 x 296
      epd2in9v2.DisplayFrame_Partial();
      epd2in9v2.SetFrameMemory_Partial(gImage_IMG2, 0, 40, 128, 296);  // x,y,DATA,resolution 128 x 296
      epd2in9v2.DisplayFrame_Partial();
      epd2in9v2.SetFrameMemory_Partial(gImage_IMG3, 0, 40, 128, 296);  // x,y,DATA,resolution 128 x 296
      epd2in9v2.DisplayFrame_Partial();
    }
}
