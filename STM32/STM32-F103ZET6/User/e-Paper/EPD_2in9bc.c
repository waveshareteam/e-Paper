/*****************************************************************************
* | File      	:   EPD_2in9bc.c
* | Author      :   Waveshare team
* | Function    :   2.9inch e-paper b&c
* | Info        :
*----------------
* |	This version:   V3.0
* | Date        :   2019-06-12
* | Info        :
* -----------------------------------------------------------------------------
* V3.0(2019-06-12):
* 1.Change:
*    EPD_Reset() => EPD_2IN9BC_Reset()
*    EPD_SendCommand() => EPD_2IN9BC_SendCommand()
*    EPD_SendData() => EPD_2IN9BC_SendData()
*    EPD_WaitUntilIdle() => EPD_2IN9BC_ReadBusy()
*    EPD_Init() => EPD_2IN9BC_Init()
*    EPD_Clear() => EPD_2IN9BC_Clear()
*    EPD_Display() => EPD_2IN9BC_Display()
*    EPD_Sleep() => EPD_2IN9BC_Sleep()
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
*    #define PLL_CONTROL                                 0x30
*    #define TEMPERATURE_SENSOR_COMMAND                  0x40
*    #define TEMPERATURE_SENSOR_CALIBRATION              0x41
*    #define TEMPERATURE_SENSOR_WRITE                    0x42
*    #define TEMPERATURE_SENSOR_READ                     0x43
*    #define VCOM_AND_DATA_INTERVAL_SETTING              0x50
*    #define LOW_POWER_DETECTION                         0x51
*    #define TCON_SETTING                                0x60
*    #define TCON_RESOLUTION                             0x61
*    #define GET_STATUS                                  0x71
*    #define AUTO_MEASURE_VCOM                           0x80
*    #define VCOM_VALUE                                  0x81
*    #define VCM_DC_SETTING_REGISTER                     0x82
*    #define PARTIAL_WINDOW                              0x90
*    #define PARTIAL_IN                                  0x91
*    #define PARTIAL_OUT                                 0x92
*    #define PROGRAM_MODE                                0xA0
*    #define ACTIVE_PROGRAM                              0xA1
*    #define READ_OTP_DATA                               0xA2
*    #define POWER_SAVING                                0xE3
* -----------------------------------------------------------------------------
* V2.0(2018-11-06):
* 1.Remove:ImageBuff[EPD_HEIGHT * EPD_WIDTH / 8]
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
#include "EPD_2in9bc.h"
#include "Debug.h"

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_2IN9BC_Reset(void)
{
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(200);
    DEV_Digital_Write(EPD_RST_PIN, 0);
    DEV_Delay_ms(1);
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(200);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_2IN9BC_SendCommand(UBYTE Reg)
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
static void EPD_2IN9BC_SendData(UBYTE Data)
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
void EPD_2IN9BC_ReadBusy(void)
{
    Debug("e-Paper busy\r\n");
    while(DEV_Digital_Read(EPD_BUSY_PIN) == 0) {      //LOW: idle, HIGH: busy
        DEV_Delay_ms(100);
    }
    Debug("e-Paper busy release\r\n");
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_2IN9BC_Init(void)
{
    EPD_2IN9BC_Reset();

    EPD_2IN9BC_SendCommand(0x06); // BOOSTER_SOFT_START
    EPD_2IN9BC_SendData(0x17);
    EPD_2IN9BC_SendData(0x17);
    EPD_2IN9BC_SendData(0x17);
	
    EPD_2IN9BC_SendCommand(0x04); // POWER_ON
    EPD_2IN9BC_ReadBusy();
	
    EPD_2IN9BC_SendCommand(0x00); // PANEL_SETTING
    EPD_2IN9BC_SendData(0x8F);
	
    EPD_2IN9BC_SendCommand(0x50); // VCOM_AND_DATA_INTERVAL_SETTING
    EPD_2IN9BC_SendData(0x77);
	
    EPD_2IN9BC_SendCommand(0x61); // TCON_RESOLUTION
    EPD_2IN9BC_SendData(0x80);
    EPD_2IN9BC_SendData(0x01);
    EPD_2IN9BC_SendData(0x28);
	
    EPD_2IN9BC_SendCommand(0x82); // VCM_DC_SETTING_REGISTER
    EPD_2IN9BC_SendData(0X0A);
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_2IN9BC_Clear(void)
{
    UWORD Width = (EPD_2IN9BC_WIDTH % 8 == 0)? (EPD_2IN9BC_WIDTH / 8 ): (EPD_2IN9BC_WIDTH / 8 + 1);
    UWORD Height = EPD_2IN9BC_HEIGHT;

    //send black data
    EPD_2IN9BC_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN9BC_SendData(0xFF);
        }
    }

    //send red data
    EPD_2IN9BC_SendCommand(0x13);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN9BC_SendData(0xFF);
        }
    }
		
		EPD_2IN9BC_SendCommand(0x12);
		EPD_2IN9BC_ReadBusy();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_2IN9BC_Display(const UBYTE *blackimage, const UBYTE *ryimage)
{
    UWORD Width, Height;
    Width = (EPD_2IN9BC_WIDTH % 8 == 0)? (EPD_2IN9BC_WIDTH / 8 ): (EPD_2IN9BC_WIDTH / 8 + 1);
    Height = EPD_2IN9BC_HEIGHT;

    EPD_2IN9BC_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN9BC_SendData(blackimage[i + j * Width]);
        }
    }
    EPD_2IN9BC_SendCommand(0x92);
    
    EPD_2IN9BC_SendCommand(0x13);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN9BC_SendData(ryimage[i + j * Width]);
        }
    }
    EPD_2IN9BC_SendCommand(0x92);

    EPD_2IN9BC_SendCommand(0x12);
    EPD_2IN9BC_ReadBusy();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_2IN9BC_Sleep(void)
{
    EPD_2IN9BC_SendCommand(0x02); // POWER_OFF
    EPD_2IN9BC_ReadBusy();
    EPD_2IN9BC_SendCommand(0x07); // DEEP_SLEEP
    EPD_2IN9BC_SendData(0xA5); // check code
}
