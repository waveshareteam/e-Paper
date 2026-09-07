/*
 Example guide: https://www.amebaiot.com/en/amebapro2-arduino-epaper/

 */

#include "epdif_2in9b.h"
#include "image.h"

// SPI Pin definition
// Any GPIO pins can be used for BUSY_PIN, RST_PIN, and DC_PIN. You may refer to the pinmap of the board that you are using.
#define BUSY_PIN        4
#define RST_PIN         5
#define DC_PIN          6
#define CS_PIN          SPI_SS

EpdIf2in9b epd2in9b(BUSY_PIN, RST_PIN, DC_PIN, CS_PIN);

void setup() {
    Serial.begin(115200);
    Serial.print("e-Paper init \r\n ");
    if (epd2in9b.Init() != 0) {
        Serial.print("e-Paper init failed");
        return;
    }
}

void loop() {
    DisplayImageBlack();
    DisplayImageRed();
    DisplayQRCode();
}

void DisplayImageBlack(void) {
    /* User predefined image files  (296 x 128)
    can be modified in "image.h"
    */
    epd2in9b.ClearFrame();
    epd2in9b.SetPartialWindowBlack(gImage_basemapIMG, 0, 0, 128, 296);
    epd2in9b.DisplayFrame();
    delay(4000);
}

void DisplayImageRed(void) {
    /* User predefined image files  (296 x 128)
    can be modified in "image.h"
    */
    epd2in9b.ClearFrame();
    epd2in9b.SetPartialWindowRed(gImage_IMG1, 0, 40, 128, 296);
    epd2in9b.DisplayFrame();
    delay(4000);
}

void DisplayQRCode(void) {
    epd2in9b.ClearFrame();
    epd2in9b.SetPartialWindowBlack(gImage_Realtek_QR, 0, 0, 128, 296); // fb website 296 x 128
    epd2in9b.DisplayFrame();
    delay(4000);
    epd2in9b.SetPartialWindowBlack(gImage_Facebook_QR, 0, 0, 128, 296); // official website 296 x 128
    epd2in9b.DisplayFrame();
    delay(4000);
    epd2in9b.SetPartialWindowBlack(gImage_AmebaIoT_QR, 0, 0, 128, 296); // community website 296 x 128
    epd2in9b.DisplayFrame();
    delay(4000);
}
