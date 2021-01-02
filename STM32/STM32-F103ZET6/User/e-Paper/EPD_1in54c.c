/*****************************************************************************
* | File      	:   EPD_1in54c.C
* | Author      :   Waveshare team
* | Function    :   1.54inch e-paper c
* | Info        :
*----------------
* |	This version:   V3.1
* | Date        :   2019-06-12
* | Info        :
* -----------------------------------------------------------------------------
* V3.1(2019-06-12):
* 1.Change:
*    EPD_Reset() => EPD_1IN54C_Reset()
*    EPD_SendCommand() => EPD_1IN54C_SendCommand()
*    EPD_SendData() => EPD_1IN54C_SendData()
*    EPD_WaitUntilIdle() => EPD_1IN54C_ReadBusy()
*    EPD_Init() => EPD_1IN54C_Init()
*    EPD_Clear() => EPD_1IN54C_Clear()
*    EPD_Display() => EPD_1IN54C_Display()
*    EPD_Sleep() => EPD_1IN54C_Sleep()
* 2.remove commands define:
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
* -----------------------------------------------------------------------------
* V2.0(2018-11-14):
* 1.Remove:ImageBuff[EPD_1IN54C_HEIGHT * EPD_1IN54C_WIDTH / 8]
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
#include "EPD_1in54c.h"
#include "Debug.h"

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_1IN54C_Reset(void)
{
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(100);
    DEV_Digital_Write(EPD_RST_PIN, 0);
    DEV_Delay_ms(2);
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(100);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_1IN54C_SendCommand(UBYTE Reg)
{
    DEV_Digital_Write(EPD_DC_PIN, 0);
    DEV_Digital_Write(EPD_CS_PIN, 0);
    DEV_SPI_WriteByte(Reg);
    DEV_Digital_Write(EPD_CS_PIN, 1);
}

/******************************************************************************
function :	send data
parameter:
    Data : Write data
******************************************************************************/
static void EPD_1IN54C_SendData(UBYTE Data)
{
    DEV_Digital_Write(EPD_DC_PIN, 1);
    DEV_Digital_Write(EPD_CS_PIN, 0);
    DEV_SPI_WriteByte(Data);
    DEV_Digital_Write(EPD_CS_PIN, 1);
}

/******************************************************************************
function :	Wait until the busy_pin goes LOW
parameter:
******************************************************************************/
static void EPD_1IN54C_ReadBusy(void)
{
    unsigned char busy;
    do {
        EPD_1IN54C_SendCommand(0x71);
        busy = DEV_Digital_Read(EPD_BUSY_PIN);
        busy =!(busy & 0x01);
    }	while(busy);
    DEV_Delay_ms(200);
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_1IN54C_Init(void)
{
    EPD_1IN54C_Reset();

    EPD_1IN54C_SendCommand(0x06); //boost soft start
    EPD_1IN54C_SendData(0x17);
    EPD_1IN54C_SendData(0x17);
    EPD_1IN54C_SendData(0x17);
    EPD_1IN54C_SendCommand(0x04);

    EPD_1IN54C_ReadBusy();

    EPD_1IN54C_SendCommand(0x00); //panel setting
    EPD_1IN54C_SendData(0x0f); //LUT from OTP£¬160x296
    EPD_1IN54C_SendData(0x0d); //VCOM to 0V fast

    EPD_1IN54C_SendCommand(0x61); //resolution setting
    EPD_1IN54C_SendData(0x98); //152
    EPD_1IN54C_SendData(0x00); //152
    EPD_1IN54C_SendData(0x98);

    EPD_1IN54C_SendCommand(0X50); //VCOM AND DATA INTERVAL SETTING
    EPD_1IN54C_SendData(0x77); //WBmode:VBDF 17|D7 VBDW 97 VBDB 57		WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_1IN54C_Clear(void)
{
    UWORD Width, Height;
    Width = (EPD_1IN54C_WIDTH % 8 == 0)? (EPD_1IN54C_WIDTH / 8 ): (EPD_1IN54C_WIDTH / 8 + 1);
    Height = EPD_1IN54C_HEIGHT;

    //send black data
    EPD_1IN54C_SendCommand(0x10);
    for(UWORD i = 0; i < Height; i++) {
        for(UWORD i = 0; i < Width; i++) {
            EPD_1IN54C_SendData(0xFF);
        }
    }

    //send red data
    EPD_1IN54C_SendCommand(0x13);
    for(UWORD i = 0; i < Height; i++) {
        for(UWORD i = 0; i < Width; i++) {
            EPD_1IN54C_SendData(0xFF);
        }
    }

    //Display refresh
    EPD_1IN54C_SendCommand(0x12);
    EPD_1IN54C_ReadBusy();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_1IN54C_Display(const UBYTE *blackimage, const UBYTE *redimage)
{
    UWORD Width, Height;
    Width = (EPD_1IN54C_WIDTH % 8 == 0)? (EPD_1IN54C_WIDTH / 8 ): (EPD_1IN54C_WIDTH / 8 + 1);
    Height = EPD_1IN54C_HEIGHT;

    //send black data
    EPD_1IN54C_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_1IN54C_SendData(blackimage[i + j * Width]);
        }
    }

    //send red data
    EPD_1IN54C_SendCommand(0x13);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_1IN54C_SendData(redimage[i + j * Width]);
        }
    }

    //Display refresh
    EPD_1IN54C_SendCommand(0x12);
    EPD_1IN54C_ReadBusy();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_1IN54C_Sleep(void)
{
    EPD_1IN54C_SendCommand(0X02);  	//power off
    EPD_1IN54C_ReadBusy();
    EPD_1IN54C_SendCommand(0X07);  	//deep sleep
    EPD_1IN54C_SendData(0xA5);
}
