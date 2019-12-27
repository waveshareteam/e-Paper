#include <SPI.h>
#include "epd2in13_V2.h"
#include "imagedata.h"

Epd epd;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("epd FULL");
  epd.Init(FULL);
  epd.Display(IMAGE_DATA);

  Serial.println("epd PART");
  epd.DisplayPartBaseImage(IMAGE_DATA);
  char i = 0;
  for (i = 0; i < 10; i++) {
    Serial.println("e-Paper PART IMAGE_DATA");
    epd.Init(PART);
    epd.DisplayPart(IMAGE_DATA);
    Serial.println("e-Paper PART Clear");
    epd.ClearPart();
  }
  
  epd.Init(FULL);
  Serial.println("e-Paper clear and sleep");
  epd.Clear();
  epd.Sleep();

}

void loop()
{

}
