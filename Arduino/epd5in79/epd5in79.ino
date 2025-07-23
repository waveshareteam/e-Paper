/**
 *  @filename   :   epd5in79-demo.ino
 *  @brief      :   5.79inch e-paper display demo
 *  @author     :   MyMX from Waveshare
 *
 *  Copyright (C) Waveshare     2024/03/06
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <SPI.h>
#include "epd5in79.h"
#include "imagedata.h"

void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
    Epd epd;
    if (epd.Init() != 0) {
        Serial.println("e-Paper init failed");
        return;
    }
    Serial.print("e-Paper Clear\r\n ");
    epd.Clear();
    
    Serial.print("e-Paper Display\r\n ");
    epd.Display(IMAGE_DATA);

    Serial.print("e-Paper Clear\r\n ");
    epd.Clear();
}

void loop() {
  // put your main code here, to run repeatedly:

}
