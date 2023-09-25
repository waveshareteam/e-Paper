/**
 *  @filename   :   epd4in2_V2.h
 *  @brief      :   Header file for Dual-color e-paper library epd4in2.cpp
 *  @author     :   Yehui from Waveshare
 *  
 *  Copyright (C) Waveshare     August 10 2017
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

#ifndef EPD4IN2_H
#define EPD4IN2_H

#include "epdif.h"

// Display resolution
#define EPD_WIDTH       400
#define EPD_HEIGHT      300

#define Seconds_1_5S      0
#define Seconds_1S        1


extern const unsigned char LUT_ALL[];

class Epd : EpdIf {
public:
    unsigned int width;
    unsigned int height;

    Epd();
    ~Epd();
    int  Init(void);
    int  Init_Fast(char mode);
    void Lut(void);
	int  Init_4Gray(void);
    void SendCommand(unsigned char command);
    void SendData(unsigned char data);
    void ReadBusy(void);
    void Reset(void);
    void TurnOnDisplay(void);
    void TurnOnDisplay_Fast(void);
    void TurnOnDisplay_Partial(void);
    void TurnOnDisplay_4Gray(void);
    void Clear(void);
    void Display(const unsigned char* Image);
    void Display_Fast(const unsigned char* Image);
    void Display_Base(const unsigned char* Image);
    void Display_Partial(unsigned char* Image, unsigned int  Xstart, unsigned int  Ystart, unsigned int  Xend, unsigned int  Yend);
    void Display_Partial_Not_refresh(unsigned char* Image, unsigned int  Xstart, unsigned int  Ystart, unsigned int  Xend, unsigned int  Yend);
    void Set_4GrayDisplay(const unsigned char *Image, int x, int y, int w, int l);
    void Sleep(void);
	
	

private:
    unsigned int reset_pin;
    unsigned int dc_pin;
    unsigned int cs_pin;
    unsigned int busy_pin;
};

#endif /* EPD4IN2_H */

/* END OF FILE */
