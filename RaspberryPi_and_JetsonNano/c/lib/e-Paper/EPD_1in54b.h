/*****************************************************************************
* | File      	:   EPD_1in54b.h
* | Author      :   Waveshare team
* | Function    :   1.54inch e-paper b
* | Info        :
*----------------
* |	This version:   V3.1
* | Date        :   2019-06-12
* | Info        :
* -----------------------------------------------------------------------------
* V3.1(2019-06-12):
* 1.Change:
*    lut_vcom0[] => EPD_1IN54_lut_vcom0[]
*    lut_w[] => EPD_1IN54_lut_w[] 
*    lut_b[] => EPD_1IN54B_lut_b[]
*    lut_g1[] => EPD_1IN54B_lut_g1[]
*    lut_g2[] => EPD_1IN54B_lut_g2[]
*    lut_vcom1[] => EPD_1IN54B_lut_vcom1[]
*    lut_red0[] => EPD_1IN54B_lut_red0[]
*    lut_red1[] => EPD_1IN54B_lut_red1[]
*    EPD_Reset() => EPD_1IN54B_Reset()
*    EPD_SendCommand() => EPD_1IN54B_SendCommand()
*    EPD_SendData() => EPD_1IN54B_SendData()
*    EPD_WaitUntilIdle() => EPD_1IN54B_ReadBusy()
*    EPD_SetLutBw() => EPD_1IN54B_SetLutBw()
*    EPD_SetLutBw => EPD_1IN54B_SetLutBw()
*    EPD_Init() => EPD_1IN54B_Init()
*    EPD_Clear() => EPD_1IN54B_Clear()
*    EPD_Display() => EPD_1IN54B_Display()
*    EPD_Sleep() => EPD_1IN54B_Sleep()
* -----------------------------------------------------------------------------
* V3.0(2019-04-24):
* 1.remove commands define:
*   #define PANEL_SETTING                               0x00
*   #define POWER_SETTING                               0x01
*   #define POWER_OFF                                   0x02
*   #define POWER_OFF_SEQUENCE_SETTING                  0x03
*   #define POWER_ON                                    0x04
*   #define POWER_ON_MEASURE                            0x05
*   #define BOOSTER_SOFT_START                          0x06
*   #define DEEP_SLEEP                                  0x07
*   #define DATA_START_TRANSMISSION_1                   0x10
*   #define DATA_STOP                                   0x11
*   #define DISPLAY_REFRESH                             0x12
*   #define DATA_START_TRANSMISSION_2                   0x13
*   #define PLL_CONTROL                                 0x30
*   #define TEMPERATURE_SENSOR_COMMAND                  0x40
*   #define TEMPERATURE_SENSOR_CALIBRATION              0x41
*   #define TEMPERATURE_SENSOR_WRITE                    0x42
*   #define TEMPERATURE_SENSOR_READ                     0x43
*   #define VCOM_AND_DATA_INTERVAL_SETTING              0x50
*   #define LOW_POWER_DETECTION                         0x51
*   #define TCON_SETTING                                0x60
*   #define TCON_RESOLUTION                             0x61
*   #define SOURCE_AND_GATE_START_SETTING               0x62
*   #define GET_STATUS                                  0x71
*   #define AUTO_MEASURE_VCOM                           0x80
*   #define VCOM_VALUE                                  0x81
*   #define VCM_DC_SETTING_REGISTER                     0x82
*   #define PROGRAM_MODE                                0xA0
*   #define ACTIVE_PROGRAM                              0xA1
*   #define READ_OTP_DATA                               0xA2
*
* -----------------------------------------------------------------------------
* V2.0(2018-10-30):
* 1.Remove:ImageBuff[EPD_1IN54B_HEIGHT * EPD_1IN54B_WIDTH / 8]
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
#ifndef __EPD_1IN54B_H_
#define __EPD_1IN54B_H_

#include "DEV_Config.h"

// Display resolution
#define EPD_1IN54B_WIDTH       200
#define EPD_1IN54B_HEIGHT      200

void EPD_1IN54B_Init(void);
void EPD_1IN54B_Clear(void);
void EPD_1IN54B_Display(const UBYTE *blackimage, const UBYTE *redimage);
void EPD_1IN54B_Sleep(void);

#endif
