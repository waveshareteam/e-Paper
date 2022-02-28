/*****************************************************************************
* | File      	:   EPD_4in2.h
* | Author      :   Waveshare team
* | Function    :   4.2inch e-paper
* | Info        :
*----------------
* |	This version:   V3.1
* | Date        :   2019-11-14
* | Info        :
* -----------------------------------------------------------------------------
* V3.1(2019-11-14):
* 1.Add 4 grayscale drive and display program
*	 Add EPD_4IN2_4Gray_lut_vcom[]
*	 Add EPD_4IN2_4Gray_lut_ww[]
*	 Add EPD_4IN2_4Gray_lut_bw[]
*	 Add EPD_4IN2_4Gray_lut_wb
*	 Add EPD_4IN2_4Gray_lut_bb
*	 Add EPD_4IN2_Partial_SetLut()
*	 Add EPD_4IN2_4Gray_lut()
*	 Add EPD_4IN2_Init_4Gray()
*	 Add EPD_4IN2_4GrayDisplay(....)
* 2.Add partial refresh display
* 	 Add EPD_4IN2_Partial_lut_vcom1[]
* 	 Add EPD_4IN2_Partial_lut_ww1[]
* 	 Add EPD_4IN2_Partial_lut_bw1[]
* 	 Add EPD_4IN2_Partial_lut_wb1[]
* 	 Add EPD_4IN2_Partial_lut_bb1[]
* 	 Add EPD_4IN2_Partial_SetLut()
* 	 Add EPD_4IN2_PartialDisplay(...)
* 	 Poor display, no display function by default
*
* V3.0(2019-06-13):
* 1.Change:
*    lut_vcomDC[]  => EPD_4IN2_lut_vcomDC[]
*    lut_ww[] => EPD_4IN2_lut_ww[]
*    lut_bw[] => EPD_4IN2_lut_bw[]
*    lut_wb[] => EPD_4IN2_lut_wb[]
*    lut_bb[] => EPD_4IN2_lut_bb[]
*    EPD_Reset() => EPD_4IN2_Reset()
*    EPD_SendCommand() => EPD_4IN2_SendCommand()
*    EPD_SendData() => EPD_4IN2_SendData()
*    EPD_WaitUntilIdle() => EPD_4IN2_ReadBusy()
*    EPD_SetFullReg() => EPD_4IN2_SetFullReg()
*    EPD_SetPartReg() => EPD_4IN2_SetPartReg()
*    EPD_TurnOnDisplay() => EPD_4IN2_TurnOnDisplay()
*    EPD_Init() => EPD_4IN2_Init()
*    EPD_Clear() => EPD_4IN2_Clear()
*    EPD_Display() => EPD_4IN2_Display()
*    EPD_Sleep() => EPD_4IN2_Sleep()
* 2.remove commands define:
*    #define PANEL_SETTING                               0x00
*    #define POWER_SETTING                               0x01
*    #define POWER_OFF                                   0x02
*    #define POWER_OFF_SEQUENCE_SETTING                  0x03
*    #define POWER_ON                                    0x04
*    #define POWER_ON_MEASURE                            0x05
*    #define BOOSTER_SOFT_START                          0x06
*    #define DEEP_SLEEP                                  0x07
*    #define DATA_START_TRANSMISSION_1                   0x10
*    #define DATA_STOP                                   0x11
*    #define DISPLAY_REFRESH                             0x12
*    #define DATA_START_TRANSMISSION_2                   0x13
*    #define VCOM_LUT                                    0x20
*    #define W2W_LUT                                     0x21
*    #define B2W_LUT                                     0x22
*    #define W2B_LUT                                     0x23
*    #define B2B_LUT                                     0x24
*    #define PLL_CONTROL                                 0x30
*    #define TEMPERATURE_SENSOR_CALIBRATION              0x40
*    #define TEMPERATURE_SENSOR_SELECTION                0x41
*    #define TEMPERATURE_SENSOR_WRITE                    0x42
*    #define TEMPERATURE_SENSOR_READ                     0x43
*    #define VCOM_AND_DATA_INTERVAL_SETTING              0x50
*    #define LOW_POWER_DETECTION                         0x51
*    #define TCON_SETTING                                0x60
*    #define RESOLUTION_SETTING                          0x61
*    #define GET_STATUS                                  0x71
*    #define AUTO_MEASURE_VCOM                           0x80
*    #define READ_VCOM_VALUE                             0x81
*    #define VCM_DC_SETTING                              0x82
*    #define PARTIAL_WINDOW                              0x90
*    #define PARTIAL_IN                                  0x91
*    #define PARTIAL_OUT                                 0x92
*    #define PROGRAM_MODE                                0xA0
*    #define ACTIVE_PROGRAM                              0xA1
*    #define READ_OTP_DATA                               0xA2
*    #define POWER_SAVING                                0xE3
* V2.0(2018-10-30):
* 1.Remove:ImageBuff[EPD_HEIGHT * EPD_WIDTH / 8]
* 2.Change:EPD_Display(UBYTE *Image)
*   Need to pass parameters: pointer to cached data
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
#ifndef _EPD_4IN2_H_
#define _EPD_4IN2_H_

#include "DEV_Config.h"

// Display resolution
#define EPD_4IN2_WIDTH       400
#define EPD_4IN2_HEIGHT      300

void EPD_4IN2_Init_Fast(void);
void EPD_4IN2_Init_Partial(void);
void EPD_4IN2_Clear(void);
void EPD_4IN2_Display(UBYTE *Image);
void EPD_4IN2_Sleep(void);
void EPD_4IN2_PartialDisplay(UWORD X_start,UWORD Y_start,UWORD X_end,UWORD Y_end, UBYTE *Image);

void EPD_4IN2_Init_4Gray(void);
void EPD_4IN2_4GrayDisplay(const UBYTE *Image);
#endif
