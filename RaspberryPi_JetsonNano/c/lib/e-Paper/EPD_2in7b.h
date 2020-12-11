/*****************************************************************************
* | File      	:   EPD_2in7b.h
* | Author      :   Waveshare team
* | Function    :   2.7inch e-paper b
* | Info        :
*----------------
* |	This version:   V3.0
* | Date        :   2019-06-12
* | Info        :
* -----------------------------------------------------------------------------
* V3.0(2019-06-12): 
*    lut_vcom_dc[] => EPD_2in7B_lut_vcom_dc[]
*    lut_ww[] => EPD_2in7B_lut_ww[] 
*    lut_bw[] => EPD_2in7B_lut_bw[]
*    lut_bb[] => EPD_2in7B_lut_bb[]
*    lut_wb[] => EPD_2in7B_lut_wb[]
*    EPD_Reset() => EPD_2in7B_Reset()
*    EPD_SendCommand() => EPD_2in7B_SendCommand()
*    EPD_SendData() => EPD_2in7B_SendData()
*    EPD_WaitUntilIdle() => EPD_2in7B_ReadBusy()
*    EPD_SetLut() => EPD_2in7B_SetLut()
*    EPD_Init() => EPD_2in7B_Init()
*    EPD_Clear() => EPD_2in7B_Clear()
*    EPD_Display() => EPD_2in7B_Display()
*    EPD_Sleep() => EPD_2in7B_Sleep()
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
*    #define PARTIAL_DATA_START_TRANSMISSION_1           0x14
*    #define PARTIAL_DATA_START_TRANSMISSION_2           0x15
*    #define PARTIAL_DISPLAY_REFRESH                     0x16
*    #define LUT_FOR_VCOM                                0x20
*    #define LUT_WHITE_TO_WHITE                          0x21
*    #define LUT_BLACK_TO_WHITE                          0x22
*    #define LUT_WHITE_TO_BLACK                          0x23
*    #define LUT_BLACK_TO_BLACK                          0x24
*    #define PLL_CONTROL                                 0x30
*    #define TEMPERATURE_SENSOR_COMMAND                  0x40
*    #define TEMPERATURE_SENSOR_CALIBRATION              0x41
*    #define TEMPERATURE_SENSOR_WRITE                    0x42
*    #define TEMPERATURE_SENSOR_READ                     0x43
*    #define VCOM_AND_DATA_INTERVAL_SETTING              0x50
*    #define LOW_POWER_DETECTION                         0x51
*    #define TCON_SETTING                                0x60
*    #define TCON_RESOLUTION                             0x61
*    #define SOURCE_AND_GATE_START_SETTING               0x62
*    #define GET_STATUS                                  0x71
*    #define AUTO_MEASURE_VCOM                           0x80
*    #define VCOM_VALUE                                  0x81
*    #define VCM_DC_SETTING_REGISTER                     0x82
*    #define PROGRAM_MODE                                0xA0
*    #define ACTIVE_PROGRAM                              0xA1
*    #define READ_OTP_DATA                               0xA2
* -----------------------------------------------------------------------------
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
#ifndef __EPD_2IN7B_H_
#define __EPD_2IN7B_H_

#include "DEV_Config.h"

// Display resolution
#define EPD_2IN7B_WIDTH       176
#define EPD_2IN7B_HEIGHT      264

void EPD_2IN7B_Init(void);
void EPD_2IN7B_Clear(void);
void EPD_2IN7B_Display(UBYTE *Imageblack, UBYTE *Imagered);
void EPD_2IN7B_Sleep(void);

#endif
