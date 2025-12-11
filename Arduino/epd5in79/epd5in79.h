/**
 *  @filename   :   epd5in79.h
 *  @brief      :   Header file for e-paper library epd5in83.cpp
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

#ifndef EPD5IN79_H
#define EPD5IN79_H

#include "epdif.h"

// Display resolution
#define EPD_WIDTH       792
#define EPD_HEIGHT      272

class Epd : EpdIf {
public:
    Epd();
    ~Epd();
    int  Init(void);
    int  Init_Fast(void);
    void ReadBusy(void);
    void Reset(void);
    void TurnOnDisplay(void);
    void TurnOnDisplay_Fast(void);
    void TurnOnDisplay_Part(void);
    void TurnOnDisplay_4GRAY(void);
    void Display(const unsigned char* frame_buffer);
    void Display_Fast(const unsigned char* frame_buffer);
    void Clear(void);
    void SendCommand(unsigned char command);
    void SendData(unsigned char data);
    void Sleep(void);
private:
    unsigned int reset_pin;
    unsigned int dc_pin;
    unsigned int cs_pin;
    unsigned int busy_pin;
    unsigned long width;
    unsigned long height;
};

#endif /* EPD5IN83_H */

/* END OF FILE */
