/*****************************************************************************
* | File      	:   EPD_7IN5BC.h
* | Author      :   Waveshare team
* | Function    :   7.5inch e-paper b&c
* | Info        :
* |	This version:   V3.0
* | Date        :   2019-06-13
* | Info        :
* -----------------------------------------------------------------------------
* V3.0(2019-06-13):
* 1.Change:
*    EPD_Reset() => EPD_7IN5BC_Reset()
*    EPD_SendCommand() => EPD_7IN5BC_SendCommand()
*    EPD_SendData() => EPD_7IN5BC_SendData()
*    EPD_WaitUntilIdle() => EPD_7IN5BC_ReadBusy()
*    EPD_Init() => EPD_7IN5BC_Init()
*    EPD_Clear() => EPD_7IN5BC_Clear()
*    EPD_Display() => EPD_7IN5BC_Display()
*    EPD_Sleep() => EPD_7IN5BC_Sleep()
* 2.Remove:
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
*   #define IMAGE_PROCESS                               0x13
*   #define LUT_FOR_VCOM                                0x20
*   #define LUT_BLUE                                    0x21
*   #define LUT_WHITE                                   0x22
*   #define LUT_GRAY_1                                  0x23
*   #define LUT_GRAY_2                                  0x24
*   #define LUT_RED_0                                   0x25
*   #define LUT_RED_1                                   0x26
*   #define LUT_RED_2                                   0x27
*   #define LUT_RED_3                                   0x28
*   #define LUT_XON                                     0x29
*   #define PLL_CONTROL                                 0x30
*   #define TEMPERATURE_SENSOR_COMMAND                  0x40
*   #define TEMPERATURE_CALIBRATION                     0x41
*   #define TEMPERATURE_SENSOR_WRITE                    0x42
*   #define TEMPERATURE_SENSOR_READ                     0x43
*   #define VCOM_AND_DATA_INTERVAL_SETTING              0x50
*   #define LOW_POWER_DETECTION                         0x51
*   #define TCON_SETTING                                0x60
*   #define TCON_RESOLUTION                             0x61
*   #define SPI_FLASH_CONTROL                           0x65
*   #define REVISION                                    0x70
*   #define GET_STATUS                                  0x71
*   #define AUTO_MEASUREMENT_VCOM                       0x80
*   #define READ_VCOM_VALUE                             0x81
*   #define VCM_DC_SETTING                              0x82
* 3.add:
*   EPD_7IN5BC_DisplayHalfScreen()
* -----------------------------------------------------------------------------
* V2.0(2018-11-09):
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
#ifndef __EPD_7IN5BC_H_
#define __EPD_7IN5BC_H_

#include "DEV_Config.h"

// Display resolution
#define EPD_7IN5BC_WIDTH       640
#define EPD_7IN5BC_HEIGHT      384

void EPD_7IN5BC_Init(void);
void EPD_7IN5BC_Clear(void);
void EPD_7IN5BC_Display(const UBYTE *blackimage, const UBYTE *ryimage);
void EPD_7IN5BC_DisplayHalfScreen(const UBYTE *blackimage, const UBYTE *ryimage);
void EPD_7IN5BC_Sleep(void);

#endif
