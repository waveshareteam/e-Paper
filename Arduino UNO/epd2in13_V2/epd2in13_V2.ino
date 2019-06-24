#include <SPI.h>
#include "epd2in13_V2.h"
#include "imagedata.h"

Epd epd;

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);
    if (epd.Init(FULL) != 0) {
        Serial.println("e-Paper init failed");
        return;
    }

    Serial.println("e-Paper clear");
    epd.Clear();

    Serial.println("e-Paper show pic");
    epd.Display(IMAGE_DATA);

    Serial.println("e-Paper clear and sleep");
    epd.Clear();
    epd.Sleep();

}

void loop()
{

}

