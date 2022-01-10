/**
    @filename   :   epd3in7-demo.ino
    @brief      :   3.7inch e-paper display demo
    @author     :   Waveshare

    Copyright (C) Waveshare     July 17 2020

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
#include "epd3in7.h"
#include "imagedata.h"
#include "epdpaint.h"

#define COLORED     0
#define UNCOLORED   1

UBYTE image[700];
Paint paint(image, 0, 0);    // width should be the multiple of 8 
UDOUBLE time_start_ms;
UDOUBLE time_now_s;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    Epd epd;
    if (epd.Init() != 0) {
        Serial.print("e-Paper init failed");
        return;
    }
    Serial.print("3.7inch e-paper demo\r\n ");
    Serial.print("e-Paper Clear\r\n ");
    epd.Clear(1);  
    Serial.print("draw image\r\n ");
    epd.DisplayFrame(IMAGE_DATA, true);   // Set base image
    delay(3000);
    
    paint.SetWidth(40);
    paint.SetHeight(120);
    paint.SetRotate(ROTATE_270);
    paint.Clear(UNCOLORED);

    UBYTE i;
    time_start_ms = millis();
    for(i=0; i<5; i++) {
        time_now_s = (millis() - time_start_ms) / 1000;
        char time_string[] = {'0', '0', ':', '0', '0', '\0'};
        time_string[0] = time_now_s / 60 / 10 + '0';
        time_string[1] = time_now_s / 60 % 10 + '0';
        time_string[3] = time_now_s % 60 / 10 + '0';
        time_string[4] = time_now_s % 60 % 10 + '0';

        paint.Clear(UNCOLORED);
        paint.DrawStringAt(20, 10, time_string, &Font16, COLORED);
        Serial.print("refresh------\r\n ");
        // epd.DisplayFrame_Partial(paint.GetImage(), 20, 100, 40, 120); // Width must be a multiple of 8
        /* Writes new data to RAM */
        epd.DisplayFrame_Part(paint.GetImage(), 40+i*40, 30, 80+i*40, 140, false);   // Xstart must be a multiple of 8
        /* Displays and toggles the RAM currently in use */
        epd.TurnOnDisplay();
        /* Writes the last data to another RAM */
        epd.DisplayFrame_Part(paint.GetImage(), 40+i*40, 30, 80+i*40, 140, false);   // Xstart must be a multiple of 8
        delay(500);
    }
    
    Serial.print("clear and sleep......\r\n ");
    epd.Init();
    epd.Clear(1);
    epd.Sleep();
}

void loop() {
    // put your main code here, to run repeatedly:

}
