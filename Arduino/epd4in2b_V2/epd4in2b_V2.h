/**
 *  @filename   :   epd4in2_V2.h
 *  @brief      :   Header file for Dual-color e-paper library epd4in2.cpp
 *  @author     :   Yehui from Waveshare
 *  
 *  Copyright (C) Waveshare     Nov 11 2020
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

#ifndef EPD4IN2_V2_H
#define EPD4IN2_V2_H

#include "epdif.h"

// Display resolution
#define EPD_WIDTH       400
#define EPD_HEIGHT      300

#define UWORD  unsigned int
#define UBYTE  unsigned char

class Epd : EpdIf {
public:
    unsigned int width;
    unsigned int height;
    

    Epd();
    ~Epd();
    int Init_new(void);
    int Init_old(void);
    int  Init(void);
    void SendCommand(unsigned char command);
    void SendData(unsigned char data);
    void ReadBusy(void);
    void Reset(void);
    void Display_Window_Black(const UBYTE *image, UBYTE count);
    void Display_Window_Red_new(const UBYTE *image, UBYTE count);
    void Display_Window_Red_old(const UBYTE *image, UBYTE count);
    void Display_Window_Red(const UBYTE *image, UBYTE count);
    void DisplayFrame(void);
    void Display_old(const UBYTE *blackimage, const UBYTE *ryimage);
    void Display_new(const UBYTE *blackimage, const UBYTE *ryimage);
    void Display(const UBYTE *blackimage, const UBYTE *ryimage);
    void Clear_new();
    void Clear_old();
    void Clear();
    void Sleep_new(void);
    void Sleep_old();
    void Sleep(void);

private:
    unsigned int reset_pin;
    unsigned int dc_pin;
    unsigned int cs_pin;
    unsigned int busy_pin;
    unsigned char flag;
};

#endif /* EPD4IN2_H */

/* END OF FILE */
