/*****************************************************************************
* | File      	:   EPD_2in7.c
* | Author      :   Waveshare team
* | Function    :   2.7inch e-paper
* | Info        :
*----------------
* |	This version:   V3.0
* | Date        :   2019-06-12
* | Info        :
* -----------------------------------------------------------------------------
* V3.0(2019-06-12):
* 1.Change:
*    lut_vcom_dc[] => EPD_2in7_lut_vcom_dc[]
*    lut_ww[] => EPD_2in7_lut_ww[] 
*    lut_bw[] => EPD_2in7_lut_bw[]
*    lut_bb[] => EPD_2in7_lut_bb[]
*    lut_wb[] => EPD_2in7_lut_wb[]
*    EPD_Reset() => EPD_2in7_Reset()
*    EPD_SendCommand() => EPD_2in7_SendCommand()
*    EPD_SendData() => EPD_2in7_SendData()
*    EPD_WaitUntilIdle() => EPD_2in7_ReadBusy()
*    EPD_SetLut() => EPD_2in7_SetLut()
*    EPD_Init() => EPD_2in7_Init()
*    EPD_Clear() => EPD_2in7_Clear()
*    EPD_Display() => EPD_2in7_Display()
*    EPD_Sleep() => EPD_2in7_Sleep()
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
* V2.0(2018-11-06):
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
#include "EPD_2in7.h"
#include "Debug.h"

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_2in7_Reset(void)
{
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(200);
    DEV_Digital_Write(EPD_RST_PIN, 0);
    DEV_Delay_ms(10);
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(200);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_2in7_SendCommand(UBYTE Reg)
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
static void EPD_2in7_SendData(UBYTE Data)
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
static void EPD_2in7_ReadBusy(void)
{
    Debug("e-Paper busy\r\n");
    UBYTE busy;
    do {
        EPD_2in7_SendCommand(0x71);
        busy = DEV_Digital_Read(EPD_BUSY_PIN);
        busy =!(busy & 0x01);
    } while(busy);
    DEV_Delay_ms(200);
    Debug("e-Paper busy release\r\n");
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_2IN7_Init(void)
{
    EPD_2in7_Reset();

		EPD_2in7_SendCommand(0x06); //boost soft start
		EPD_2in7_SendData(0x07); //A
		EPD_2in7_SendData(0x07); //B
		EPD_2in7_SendData(0x17); //C       

		EPD_2in7_SendCommand(0x04);  
		EPD_2in7_ReadBusy();

		EPD_2in7_SendCommand(0x00); //panel setting
		EPD_2in7_SendData(0x1f); //LUT from OTP¡ê?128x296
	
		EPD_2in7_SendCommand(0x16);
		EPD_2in7_SendData(0x00); //KW-BF   KWR-AF	BWROTP 0f	
	
		EPD_2in7_SendCommand(0xF8); 
		EPD_2in7_SendData(0x60);
		EPD_2in7_SendData(0xa5);

		EPD_2in7_SendCommand(0xF8);
		EPD_2in7_SendData(0x73);
		EPD_2in7_SendData(0x23);

		EPD_2in7_SendCommand(0xF8); 
		EPD_2in7_SendData(0x7C);
		EPD_2in7_SendData(0x00);
		
		EPD_2in7_SendCommand(0X50);
		EPD_2in7_SendData(0x97); //WBmode:VBDF 17|D7 VBDW 97 VBDB 57		WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7

}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_2IN7_Clear(void)
{
    UWORD Width, Height;
    Width = (EPD_2IN7_WIDTH % 8 == 0)? (EPD_2IN7_WIDTH / 8 ): (EPD_2IN7_WIDTH / 8 + 1);
    Height = EPD_2IN7_HEIGHT;

    EPD_2in7_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2in7_SendData(0XFF);
        }
    }

    EPD_2in7_SendCommand(0x13);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2in7_SendData(0XFF);
        }
    }
		
    EPD_2in7_SendCommand(0x12);
    EPD_2in7_ReadBusy();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_2IN7_Display(UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_2IN7_WIDTH % 8 == 0)? (EPD_2IN7_WIDTH / 8 ): (EPD_2IN7_WIDTH / 8 + 1);
    Height = EPD_2IN7_HEIGHT;

    EPD_2in7_SendCommand(0x13);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2in7_SendData(Image[i + j * Width]);
        }
    }
    EPD_2in7_SendCommand(0x12);
    EPD_2in7_ReadBusy();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_2IN7_Sleep(void)
{
    EPD_2in7_SendCommand(0X02);  	//power off
    EPD_2in7_SendCommand(0X07);  	//deep sleep
    EPD_2in7_SendData(0xA5);
}
