/*****************************************************************************
* | File      	:   EPD_2in13d.h
* | Author      :   Waveshare team
* | Function    :   2.13inch e-paper d
* | Info        :
*----------------
* |	This version:   V3.0
* | Date        :   2019-06-12
* | Info        :
* -----------------------------------------------------------------------------
* V3.0(2019-06-12):
* 1.Change:
*    lut_vcomDC[]  => EPD_2IN13D_lut_vcomDC[]
*    lut_ww[] => EPD_2IN13D_lut_ww[]
*    lut_bw[] => EPD_2IN13D_lut_bw[]
*    lut_wb[] => EPD_2IN13D_lut_wb[]
*    lut_bb[] => EPD_2IN13D_lut_bb[]
*    lut_vcom1[] => EPD_2IN13D_lut_vcom1[]
*    lut_ww1[] => EPD_2IN13D_lut_ww1[]
*    lut_bw1[] => EPD_2IN13D_lut_bw1[]
*    lut_wb1[] => EPD_2IN13D_lut_wb1[]
*    lut_bb1[] => EPD_2IN13D_lut_bb1[]
*    EPD_Reset() => EPD_2IN13D_Reset()
*    EPD_SendCommand() => EPD_2IN13D_SendCommand()
*    EPD_SendData() => EPD_2IN13D_SendData()
*    EPD_WaitUntilIdle() => EPD_2IN13D_ReadBusy()
*    EPD_SetFullReg() => EPD_2IN13D_SetFullReg()
*    EPD_SetPartReg() => EPD_2IN13D_SetPartReg()
*    EPD_TurnOnDisplay() => EPD_2IN13D_TurnOnDisplay()
*    EPD_Init() => EPD_2IN13D_Init()
*    EPD_Clear() => EPD_2IN13D_Clear()
*    EPD_Display() => EPD_2IN13D_Display()
*    EPD_Sleep() => EPD_2IN13D_Sleep()
* V2.0(2018-11-13):
* 1.Remove:ImageBuff[EPD_2IN13D_HEIGHT * EPD_2IN13D_WIDTH / 8]
* 2.Change:EPD_Display(UBYTE *Image)
*   Need to pass parameters: pointer to cached data
* 3.Change:
*   EPD_RST -> EPD_RST_PIN
*   EPD_DC -> EPD_DC_PIN
*   EPD_CS -> EPD_CS_PIN
*   EPD_BUSY -> EPD_BUSY_PIN
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
#ifndef __EPD_2IN13D_H_
#define __EPD_2IN13D_H_

#include "DEV_Config.h"

// Display resolution
#define EPD_2IN13D_WIDTH   104
#define EPD_2IN13D_HEIGHT  212

void EPD_2IN13D_Init(void);
void EPD_2IN13D_Clear(void);
void EPD_2IN13D_Display(UBYTE *Image);
void EPD_2IN13D_DisplayPart(UBYTE *Image);
void EPD_2IN13D_Sleep(void);

#endif
