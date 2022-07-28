/**
 *  @filename   :   epd7in3g.h
 *  @brief      :   Header file for e-paper display library epd7in3g.cpp
 *  @author     :   Waveshare
 *  
 *  Copyright (C) Waveshare     2022/7/22
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

#ifndef EPD1IN64G_H
#define EPD1IN64G_H

#include "epdif.h"

// Display resolution
#define EPD_WIDTH       800
#define EPD_HEIGHT      480

//colour
#define  black   0x00 
#define  white   0x55
#define  yellow  0xAA
#define  red     0xFF

#define UWORD   unsigned int
#define UBYTE   unsigned char
#define UDOUBLE unsigned long

class Epd : EpdIf {
public:
    unsigned long WIDTH;
    unsigned long HEIGHT;

    Epd();
    ~Epd();
    int  Init();
    void SendCommand(unsigned char command);
    void SendData(unsigned char data);
    void Reset(void);
    void ReadBusyH(void);
    void ReadBusyL(void);
    void TurnOnDisplay(void);
    void Clear(UBYTE color);
    void Display(UBYTE *Image);
    void Display_part(UBYTE *Image, UWORD xstart, UWORD ystart, UWORD image_width, UWORD image_heigh);
    void Sleep(void);


private:
    unsigned int reset_pin;
    unsigned int dc_pin;
    unsigned int cs_pin;
    unsigned int busy_pin;
};

#endif /* EPD1IN54_H */

/* END OF FILE */
