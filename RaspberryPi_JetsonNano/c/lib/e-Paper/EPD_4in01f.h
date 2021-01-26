/*****************************************************************************
* | File      	:   EPD_4in01f.h
* | Author      :   Waveshare team
* | Function    :   4.01inch e-paper
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2020-11-06
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
#ifndef __EPD_4IN01F_H__
#define __EPD_4IN01F_H__

#include "DEV_Config.h"

/**********************************
Color Index
**********************************/
#define EPD_4IN01F_BLACK   0x0	/// 000
#define EPD_4IN01F_WHITE   0x1	///	001
#define EPD_4IN01F_GREEN   0x2	///	010
#define EPD_4IN01F_BLUE    0x3	///	011
#define EPD_4IN01F_RED     0x4	///	100
#define EPD_4IN01F_YELLOW  0x5	///	101
#define EPD_4IN01F_ORANGE  0x6	///	110
#define EPD_4IN01F_CLEAN   0x7	///	111   unavailable  Afterimage

#define EPD_4IN01F_WIDTH       640
#define EPD_4IN01F_HEIGHT      400

void EPD_4IN01F_Clear(UBYTE color);
void EPD_4IN01F_ReClear(void);
void EPD_4IN01F_Sleep(void);
void EPD_Init(void);
void EPD_4IN01F_Show7Block(void);
void EPD_4IN01F_Display(const UBYTE *image);
void EPD_4IN01F_Init(void);
void EPD_4IN01F_Display_part(const UBYTE *image, UWORD xstart, UWORD ystart, 
								UWORD image_width, UWORD image_heigh);
#endif



