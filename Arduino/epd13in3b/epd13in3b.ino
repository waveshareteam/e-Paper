/**
 *  @filename   :   epd7in5b_V2-demo.ino
 *  @brief      :   7.5inch b V2 e-paper display demo
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare      Nov 30 2020
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
#include "epd13in3b.h"
#include "imagedata.h"

void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
    Epd epd;
    if (epd.Init() != 0) {
        Serial.print("e-Paper init failed");
        return;
    }
    // epd.Clear();
    epd.Displaypart(IMAGE_DATA,250, 100,240,103, 0);
    epd.Displaypart(IMAGE_DATA,250, 300,240,103, 1);

    epd.Sleep();
}

void loop() {
  // put your main code here, to run repeatedly:

}
