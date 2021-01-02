/*****************************************************************************
* | File      	:   EPD_5IN83BC.c
* | Author      :   Waveshare team
* | Function    :   5.83inch e-paper b&c
* | Info        :
* |	This version:   V3.0
* | Date        :   2019-06-13
* | Info        :
* -----------------------------------------------------------------------------
* V3.0(2019-06-13):
* 1.Change:
*    EPD_Reset() => EPD_5IN83BC_Reset()
*    EPD_SendCommand() => EPD_5IN83BC_SendCommand()
*    EPD_SendData() => EPD_5IN83BC_SendData()
*    EPD_WaitUntilIdle() => EPD_5IN83BC_ReadBusy()
*    EPD_Init() => EPD_5IN83BC_Init()
*    EPD_Clear() => EPD_5IN83BC_Clear()
*    EPD_Display() => EPD_5IN83BC_Display()
*    EPD_Sleep() => EPD_5IN83BC_Sleep()
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
*   EPD_5IN83BC_DisplayHalfScreen()
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
#include "EPD_5in83bc.h"
#include "Debug.h"

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_5IN83BC_Reset(void)
{
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(200);
    DEV_Digital_Write(EPD_RST_PIN, 0);
    DEV_Delay_ms(2);
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(200);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_5IN83BC_SendCommand(UBYTE Reg)
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
static void EPD_5IN83BC_SendData(UBYTE Data)
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
void EPD_5IN83BC_ReadBusy(void)
{
    UBYTE busy;
    Debug("e-Paper busy\r\n");
    do {
        EPD_5IN83BC_SendCommand(0x71);
        busy = DEV_Digital_Read(EPD_BUSY_PIN);
        busy =!(busy & 0x01);
    } while(busy);
    Debug("e-Paper busy release\r\n");
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_5IN83BC_TurnOnDisplay(void)
{
    EPD_5IN83BC_SendCommand(0x04); // POWER ON
    EPD_5IN83BC_ReadBusy();
    EPD_5IN83BC_SendCommand(0x12); // display refresh
    DEV_Delay_ms(100);
    EPD_5IN83BC_ReadBusy();

}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_5IN83BC_Init(void)
{
    EPD_5IN83BC_Reset();

    EPD_5IN83BC_SendCommand(0x01); // POWER_SETTING
    EPD_5IN83BC_SendData(0x37);
    EPD_5IN83BC_SendData(0x00);

    EPD_5IN83BC_SendCommand(0x00); // PANEL_SETTING
    EPD_5IN83BC_SendData(0xCF);
    EPD_5IN83BC_SendData(0x08);

    EPD_5IN83BC_SendCommand(0x30); // PLL_CONTROL
    EPD_5IN83BC_SendData(0x3A); //PLL:  0-15:0x3C, 15+:0x3A

    EPD_5IN83BC_SendCommand(0x82); // VCM_DC_SETTING
    EPD_5IN83BC_SendData(0x28); //all temperature  range

    EPD_5IN83BC_SendCommand(0x06); // BOOSTER_SOFT_START
    EPD_5IN83BC_SendData (0xc7);
    EPD_5IN83BC_SendData (0xcc);
    EPD_5IN83BC_SendData (0x15);

    EPD_5IN83BC_SendCommand(0x50); // VCOM AND DATA INTERVAL SETTING
    EPD_5IN83BC_SendData(0x77);

    EPD_5IN83BC_SendCommand(0x60); // TCON_SETTING
    EPD_5IN83BC_SendData(0x22);

    EPD_5IN83BC_SendCommand(0x65); // FLASH CONTROL
    EPD_5IN83BC_SendData(0x00);

    EPD_5IN83BC_SendCommand(0x61); // TCON_RESOLUTION
    EPD_5IN83BC_SendData (0x02); // source 600
    EPD_5IN83BC_SendData (0x58);
    EPD_5IN83BC_SendData (0x01); // gate 448
    EPD_5IN83BC_SendData (0xc0);

    EPD_5IN83BC_SendCommand(0xe5); // FLASH MODE
    EPD_5IN83BC_SendData(0x03);

}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_5IN83BC_Clear(void)
{
    UWORD Width, Height;
    Width = (EPD_5IN83BC_WIDTH % 8 == 0)? (EPD_5IN83BC_WIDTH / 8): (EPD_5IN83BC_WIDTH / 8 + 1);
    Height = EPD_5IN83BC_HEIGHT;

    EPD_5IN83BC_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            for(UBYTE k = 0; k < 4; k++) {
                EPD_5IN83BC_SendData(0x33);
            }
        }
    }

    EPD_5IN83BC_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_5IN83BC_Display(const UBYTE *blackimage, const UBYTE *ryimage)
{
    UBYTE Data_Black, Data_RY, Data;
    UDOUBLE i, j, Width, Height;
    Width = (EPD_5IN83BC_WIDTH % 8 == 0)? (EPD_5IN83BC_WIDTH / 8 ): (EPD_5IN83BC_WIDTH / 8 + 1);
    Height = EPD_5IN83BC_HEIGHT;

    EPD_5IN83BC_SendCommand(0x10);
    for (j = 0; j < Height; j++) {
        for (i = 0; i < Width; i++) {
            Data_Black = blackimage[i + j * Width];
            Data_RY = ryimage[i + j * Width]; // red or yellow
            for(UBYTE k = 0; k < 8; k++) {
                if ((Data_RY & 0x80) == 0x00) {
                    Data = 0x04;                //red
                } else if ((Data_Black & 0x80) == 0x00) {
                    Data = 0x00;               //black
                } else {
                    Data = 0x03;                //white
                }
                Data = (Data << 4) & 0xFF;
                Data_Black = (Data_Black << 1) & 0xFF;
                Data_RY = (Data_RY << 1) & 0xFF;
                k += 1;

                if((Data_RY & 0x80) == 0x00) {
                    Data |= 0x04;              //red
                } else if ((Data_Black & 0x80) == 0x00) {
                    Data |= 0x00;              //black
                } else {
                    Data |= 0x03;              //white
                }
                Data_Black = (Data_Black << 1) & 0xFF;
                Data_RY = (Data_RY << 1) & 0xFF;
                EPD_5IN83BC_SendData(Data);
            }
        }
    }

    EPD_5IN83BC_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_5IN83BC_DisplayHalfScreen(const UBYTE *blackimage, const UBYTE *ryimage)
{
    UBYTE Data_Black, Data_RY, Data;
    UDOUBLE i, j, Width, Height;
    Width = (EPD_5IN83BC_WIDTH % 8 == 0)? (EPD_5IN83BC_WIDTH / 8 ): (EPD_5IN83BC_WIDTH / 8 + 1);
    Height = EPD_5IN83BC_HEIGHT;

    EPD_5IN83BC_SendCommand(0x10);
    for (j = 0; j < Height / 2; j++) {
        for (i = 0; i < Width; i++) {
            Data_Black = blackimage[i + j * Width];
            Data_RY = ryimage[i + j * Width]; // red or yellow
            for(UBYTE k = 0; k < 8; k++) {
                if ((Data_RY & 0x80) == 0x00) {
                    Data = 0x04;                //red
                } else if ((Data_Black & 0x80) == 0x00) {
                    Data = 0x00;               //black
                } else {
                    Data = 0x03;                //white
                }
                Data = (Data << 4) & 0xFF;
                Data_Black = (Data_Black << 1) & 0xFF;
                Data_RY = (Data_RY << 1) & 0xFF;
                k += 1;

                if((Data_RY & 0x80) == 0x00) {
                    Data |= 0x04;              //red
                } else if ((Data_Black & 0x80) == 0x00) {
                    Data |= 0x00;              //black
                } else {
                    Data |= 0x03;              //white
                }
                Data_Black = (Data_Black << 1) & 0xFF;
                Data_RY = (Data_RY << 1) & 0xFF;
                EPD_5IN83BC_SendData(Data);
            }
        }
    }

    for (j = 0; j < Height / 2; j++) {
        for (i = 0; i < Width; i++) {
            for(UBYTE k = 0; k < 4; k++) {
                EPD_5IN83BC_SendData(0x33);
            }
        }
    }

    EPD_5IN83BC_TurnOnDisplay();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_5IN83BC_Sleep(void)
{
    EPD_5IN83BC_SendCommand(0x02); // POWER_OFF
    EPD_5IN83BC_ReadBusy();
    EPD_5IN83BC_SendCommand(0x07); // DEEP_SLEEP
    EPD_5IN83BC_SendData(0XA5);
}
