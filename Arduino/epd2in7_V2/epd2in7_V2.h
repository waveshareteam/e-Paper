/**
 *  @filename   :   epd2in7_V2.h
 *  @brief      :   Header file for e-paper library epd2in7_V2.cpp
 *  @author     :   Waveshare
 *  
 *  Copyright (C) Waveshare     September 20 2022
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

#ifndef EPD2IN7_V2_H
#define EPD2IN7_V2_H

#include "epdif.h"

// Display resolution
#define EPD_WIDTH       176
#define EPD_HEIGHT      264


class Epd : EpdIf {
public:
    unsigned int WIDTH;
    unsigned int HEIGHT;

    Epd();
    ~Epd();
    int  Init(void);
    int  Init_Fast(void);
    void Init_4Gray(void);
    void SendCommand(unsigned char command);
    void SendData(unsigned char data);
    void ReadBusy(void);
    void Reset(void);
    void Lut(void);
    void TurnOnDisplay(void);
    void TurnOnDisplay_Fast(void);
    void TurnOnDisplay_Partial(void);
    void TurnOnDisplay_4GRAY(void);
    void Clear(void);
    void Display(const unsigned char* Image);
    void Display_Fast(const unsigned char* Image);
    void Display_Base(const unsigned char* Image);
    void Display_Base_color(unsigned char color);
    void Display_Partial(unsigned char* Image, unsigned int  Xstart, unsigned int  Ystart, unsigned int  Xend, unsigned int  Yend);
    void Display_Partial_Not_refresh(unsigned char* Image, unsigned int  Xstart, unsigned int  Ystart, unsigned int  Xend, unsigned int  Yend);
    void Display4Gray(const unsigned char *Image);
    void Sleep(void);

private:
    unsigned int reset_pin;
    unsigned int dc_pin;
    unsigned int cs_pin;
    unsigned int busy_pin;
};

#endif /* EPD2IN7_H */

/* END OF FILE */
