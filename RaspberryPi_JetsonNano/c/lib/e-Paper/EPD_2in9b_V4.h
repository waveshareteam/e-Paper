/*****************************************************************************
* | File      	:   EPD_2in9b V4.h
* | Author      :   Waveshare team
* | Function    :   2.9inch e-paper b V4
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2023-12-18
* | Info        :
* -----------------------------------------------------------------------------
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
#ifndef __EPD_2IN9B_V4_H_
#define __EPD_2IN9B_V4_H_

#include "DEV_Config.h"

// Display resolution
#define EPD_2IN9B_V4_WIDTH       128
#define EPD_2IN9B_V4_HEIGHT      296

void EPD_2IN9B_V4_Init(void);
void EPD_2IN9B_V4_Init_Fast(void);
void EPD_2IN9B_V4_Clear_Fast(void);
void EPD_2IN9B_V4_Clear_Black_Fast(void);
void EPD_2IN9B_V4_Clear_Red_Fast(void);
void EPD_2IN9B_V4_Clear(void);
void EPD_2IN9B_V4_Display(const UBYTE *blackimage, const UBYTE *ryimage);
void EPD_2IN9B_V4_Display_Fast(const UBYTE *blackimage, const UBYTE *ryimage);
void EPD_2IN9B_V4_Display_Base(const UBYTE *blackimage, const UBYTE *ryimage);
void EPD_2IN9B_V4_Display_Partial(const UBYTE *Image, UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend);
void EPD_2IN9B_V4_Sleep(void);

#endif
