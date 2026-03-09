/*****************************************************************************
* | File      	:   EPD_3in97.h
* | Author      :   Waveshare team
* | Function    :   1.54inch e-paper V2
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2019-06-11
* | Info        :   
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#ifndef __EPD_3IN97_H_
#define __EPD_3IN97_H_

#include "DEV_Config.h"

// Display resolution
#define EPD_3in97_WIDTH       800
#define EPD_3in97_HEIGHT      480

void EPD_3in97_Init(void);
void EPD_3in97_Init_Fast(void);
void EPD_3in97_Init_Partial(void);
void EPD_3in97_Init_4GRAY(void);
void EPD_3in97_Clear(void);
void EPD_3in97_Clear_Black(void);
void EPD_3in97_Display(const UBYTE *Image);
void EPD_3in97_Display_Base(const UBYTE *Image);
void EPD_3in97_Display_Fast(const UBYTE *Image);
void EPD_3in97_Display_Fast_Base(const UBYTE *Image);
void EPD_3in97_Display_Partial(const UBYTE *Image, UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend);
void EPD_3in97_Display_4Gray(const UBYTE *Image);
void EPD_3in97_Sleep(void);

#endif
