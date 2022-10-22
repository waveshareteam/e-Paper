/**
    @filename   :   EPD_7in3f.ino
    @brief      :   EPD_7in3 e-paper F display demo
    @author     :   Waveshare

    Copyright (C) Waveshare     10 21 2022

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documnetation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to  whom the Software is
   furished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
*/

#include <SPI.h>
#include "imagedata.h"
#include "epd7in3f.h"

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    Epd epd;
    if (epd.Init() != 0) {
        Serial.print("e-Paper init failed");
        return;
    }

    Serial.print("e-Paper Clear\r\n ");
    epd.Clear(EPD_7IN3F_WHITE);

    Serial.print("Show pic\r\n ");
    epd.EPD_7IN3F_Display_part(gImage_7in3f, 250, 150, 300, 180);
    delay(2000);

    Serial.print("draw 7 color block\r\n ");
    epd.EPD_7IN3F_Show7Block();
    delay(2000);
    
    epd.Sleep();
}

void loop() {
    // put your main code here, to run repeatedly:

}
