/**
 *  @filename   :   epd2in36g-demo.ino
 *  @brief      :   2.36inch e-Paper (G) display demo
 *  @author     :   Waveshare
 *
 *  Copyright (C) Waveshare     2022/08/17
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
#include "epd2in36g.h"
#include "imagedata.h"

Epd epd;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("e-Paper init  \r\n");
  if (epd.Init() != 0) {
      Serial.print("e-Paper init failed");
      return;
  }

  Serial.print("White \r\n");
  epd.Clear(white);
  delay(2000);

  Serial.print("Small Image \r\n");
  epd.Display_part(Image4color, 0, 64, 168, 168);
  delay(2000);

  Serial.print("Clear...\r\n");
  epd.Clear(white);
  delay(2000);

  Serial.print("Goto Sleep...\r\n");
  epd.Sleep();
}

void loop() {

}
