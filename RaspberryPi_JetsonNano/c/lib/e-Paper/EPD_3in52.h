/*****************************************************************************
* | File      	:   EPD_3IN52.h
* | Author      :   Waveshare team
* | Function    :   3.52inch e-paper
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2022-05-07
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
#ifndef __EPD_3IN52_H_
#define __EPD_3IN52_H_

#include "DEV_Config.h"

// Display resolution
#define EPD_3IN52_WIDTH       240
#define EPD_3IN52_HEIGHT      360 

#define LUTGC_TEST          //
#define LUTDU_TEST          //

#define EPD_3IN52_WHITE                         0xFF  // 
#define EPD_3IN52_BLACK                         0x00  //
#define EPD_3IN52_Source_Line                   0xAA  //
#define EPD_3IN52_Gate_Line                     0x55  //
#define EPD_3IN52_UP_BLACK_DOWN_WHITE           0xF0  //
#define EPD_3IN52_LEFT_BLACK_RIGHT_WHITE        0x0F  //
#define EPD_3IN52_Frame                         0x01  // 
#define EPD_3IN52_Crosstalk                     0x02  //
#define EPD_3IN52_Chessboard                    0x03  //
#define EPD_3IN52_Image                         0x04  //


extern unsigned char EPD_3IN52_Flag;

void EPD_3IN52_SendCommand(UBYTE Reg);
void EPD_3IN52_SendData(UBYTE Data);
void EPD_3IN52_refresh(void);
void EPD_3IN52_lut_GC(void);
void EPD_3IN52_lut_DU(void);
void EPD_3IN52_Init(void);
void EPD_3IN52_display(UBYTE* picData);
void EPD_3IN52_display_NUM(UBYTE NUM);
void EPD_3IN52_Clear(void);
void EPD_3IN52_sleep(void);



#endif
