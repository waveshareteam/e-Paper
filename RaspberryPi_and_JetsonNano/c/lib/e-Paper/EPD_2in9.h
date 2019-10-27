/*****************************************************************************
* | File      	:   EPD_2in9.h
* | Author      :   Waveshare team
* | Function    :   2.9inch e-paper
* | Info        :
*----------------
* |	This version:   V3.0
* | Date        :   2019-06-12
* | Info        :
* -----------------------------------------------------------------------------
* V3.0(2019-06-12):
* 1.Change:
*    lut_full_update[] => EPD_2IN9_lut_full_update[]
*    lut_partial_update[] => EPD_2IN9_lut_partial_update[] 
*    EPD_Reset() => EPD_2IN9_Reset()
*    EPD_SendCommand() => EPD_2IN9_SendCommand()
*    EPD_SendData() => EPD_2IN9_SendData()
*    EPD_WaitUntilIdle() => EPD_2IN9_ReadBusy()
*    EPD_SetLut() => EPD_2IN9_SetLut()
*    EPD_SetWindow() => EPD_2IN9_SetWindow()
*    EPD_SetCursor() => EPD_2IN9_SetCursor()
*    EPD_TurnOnDisplay() => EPD_2IN9_TurnOnDisplay()
*    EPD_Init() => EPD_2IN9_Init()
*    EPD_Clear() => EPD_2IN9_Clear()
*    EPD_Display() => EPD_2IN9_Display()
*    EPD_Sleep() => EPD_2IN9_Sleep()
* 2.remove commands define:
*    #define DRIVER_OUTPUT_CONTROL                       0x01
*    #define BOOSTER_SOFT_START_CONTROL                  0x0C
*    #define GATE_SCAN_START_POSITION                    0x0F
*    #define DEEP_SLEEP_MODE                             0x10
*    #define DATA_ENTRY_MODE_SETTING                     0x11
*    #define SW_RESET                                    0x12
*    #define TEMPERATURE_SENSOR_CONTROL                  0x1A
*    #define MASTER_ACTIVATION                           0x20
*    #define DISPLAY_UPDATE_CONTROL_1                    0x21
*    #define DISPLAY_UPDATE_CONTROL_2                    0x22
*    #define WRITE_RAM                                   0x24
*    #define WRITE_VCOM_REGISTER                         0x2C
*    #define WRITE_LUT_REGISTER                          0x32
*    #define SET_DUMMY_LINE_PERIOD                       0x3A
*    #define SET_GATE_TIME                               0x3B
*    #define BORDER_WAVEFORM_CONTROL                     0x3C
*    #define SET_RAM_X_ADDRESS_START_END_POSITION        0x44
*    #define SET_RAM_Y_ADDRESS_START_END_POSITION        0x45
*    #define SET_RAM_X_ADDRESS_COUNTER                   0x4E
*    #define SET_RAM_Y_ADDRESS_COUNTER                   0x4F
*    #define TERMINATE_FRAME_READ_WRITE                  0xFF
* -----------------------------------------------------------------------------
* V2.0(2018-11-06):
* 1.Remove:ImageBuff[EPD_2IN9_HEIGHT * EPD_2IN9_WIDTH / 8]
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
#ifndef __EPD_2IN9_H_
#define __EPD_2IN9_H_

#include "DEV_Config.h"

// Display resolution
#define EPD_2IN9_WIDTH       128
#define EPD_2IN9_HEIGHT      296

#define EPD_2IN9_FULL			0
#define EPD_2IN9_PART			1

void EPD_2IN9_Init(UBYTE Mode);
void EPD_2IN9_Clear(void);
void EPD_2IN9_Display(UBYTE *Image);
void EPD_2IN9_Sleep(void);

#endif
