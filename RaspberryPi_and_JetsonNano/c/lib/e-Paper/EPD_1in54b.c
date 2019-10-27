/*****************************************************************************
* | File      	:   EPD_1in54b.c
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
#include "EPD_1in54b.h"
#include "Debug.h"

const unsigned char EPD_1IN54B_lut_vcom0[] = {0x0E, 0x14, 0x01, 0x0A, 0x06, 0x04, 0x0A, 0x0A, 0x0F, 0x03, 0x03, 0x0C, 0x06, 0x0A, 0x00};
const unsigned char EPD_1IN54B_lut_w[] = {0x0E, 0x14, 0x01, 0x0A, 0x46, 0x04, 0x8A, 0x4A, 0x0F, 0x83, 0x43, 0x0C, 0x86, 0x0A, 0x04};
const unsigned char EPD_1IN54B_lut_b[] = {0x0E, 0x14, 0x01, 0x8A, 0x06, 0x04, 0x8A, 0x4A, 0x0F, 0x83, 0x43, 0x0C, 0x06, 0x4A, 0x04};
const unsigned char EPD_1IN54B_lut_g1[] = {0x8E, 0x94, 0x01, 0x8A, 0x06, 0x04, 0x8A, 0x4A, 0x0F, 0x83, 0x43, 0x0C, 0x06, 0x0A, 0x04};
const unsigned char EPD_1IN54B_lut_g2[] = {0x8E, 0x94, 0x01, 0x8A, 0x06, 0x04, 0x8A, 0x4A, 0x0F, 0x83, 0x43, 0x0C, 0x06, 0x0A, 0x04};
const unsigned char EPD_1IN54B_lut_vcom1[] = {0x03, 0x1D, 0x01, 0x01, 0x08, 0x23, 0x37, 0x37, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const unsigned char EPD_1IN54B_lut_red0[] = {0x83, 0x5D, 0x01, 0x81, 0x48, 0x23, 0x77, 0x77, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const unsigned char EPD_1IN54B_lut_red1[] = {0x03, 0x1D, 0x01, 0x01, 0x08, 0x23, 0x37, 0x37, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_1IN54B_Reset(void)
{
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(100);
    DEV_Digital_Write(EPD_RST_PIN, 0);
    DEV_Delay_ms(10);
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(100);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_1IN54B_SendCommand(UBYTE Reg)
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
static void EPD_1IN54B_SendData(UBYTE Data)
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
static void EPD_1IN54B_ReadBusy(void)
{
    Debug("e-Paper busy\r\n");
    while(1) {
        if(DEV_Digital_Read(EPD_BUSY_PIN) == 1)
            break;
    }
    DEV_Delay_ms(200);
    Debug("e-Paper busy release\r\n");
}

/******************************************************************************
function :	Set the look-up black and white tables
parameter:
******************************************************************************/
static void EPD_1IN54B_SetLutBw(void)
{
    UWORD count;
    EPD_1IN54B_SendCommand(0x20);// g vcom
    for(count = 0; count < 15; count++) {
        EPD_1IN54B_SendData(EPD_1IN54B_lut_vcom0[count]);
    }
    EPD_1IN54B_SendCommand(0x21);// g ww --
    for(count = 0; count < 15; count++) {
        EPD_1IN54B_SendData(EPD_1IN54B_lut_w[count]);
    }
    EPD_1IN54B_SendCommand(0x22);// g bw r
    for(count = 0; count < 15; count++) {
        EPD_1IN54B_SendData(EPD_1IN54B_lut_b[count]);
    }
    EPD_1IN54B_SendCommand(0x23);// g wb w
    for(count = 0; count < 15; count++) {
        EPD_1IN54B_SendData(EPD_1IN54B_lut_g1[count]);
    }
    EPD_1IN54B_SendCommand(0x24);// g bb b
    for(count = 0; count < 15; count++) {
        EPD_1IN54B_SendData(EPD_1IN54B_lut_g2[count]);
    }
}

/******************************************************************************
function :	Set the look-up red tables
parameter:
******************************************************************************/
static void EPD_1IN54B_SetLutRed(void)
{
    UWORD count;
    EPD_1IN54B_SendCommand(0x25);
    for(count = 0; count < 15; count++) {
        EPD_1IN54B_SendData(EPD_1IN54B_lut_vcom1[count]);
    }
    EPD_1IN54B_SendCommand(0x26);
    for(count = 0; count < 15; count++) {
        EPD_1IN54B_SendData(EPD_1IN54B_lut_red0[count]);
    }
    EPD_1IN54B_SendCommand(0x27);
    for(count = 0; count < 15; count++) {
        EPD_1IN54B_SendData(EPD_1IN54B_lut_red1[count]);
    }
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_1IN54B_Init(void)
{
    EPD_1IN54B_Reset();

    EPD_1IN54B_SendCommand(0x01);// POWER_SETTING
    EPD_1IN54B_SendData(0x07);
    EPD_1IN54B_SendData(0x00);
    EPD_1IN54B_SendData(0x08);
    EPD_1IN54B_SendData(0x00);
    EPD_1IN54B_SendCommand(0x06);// BOOSTER_SOFT_START
    EPD_1IN54B_SendData(0x07);
    EPD_1IN54B_SendData(0x07);
    EPD_1IN54B_SendData(0x07);
    EPD_1IN54B_SendCommand(0x04);// POWER_ON

    EPD_1IN54B_ReadBusy();

    EPD_1IN54B_SendCommand(0X00);// PANEL_SETTING
    EPD_1IN54B_SendData(0xcf);
    EPD_1IN54B_SendCommand(0X50);// VCOM_AND_DATA_INTERVAL_SETTING
    EPD_1IN54B_SendData(0x37);// 0xF0
    EPD_1IN54B_SendCommand(0x30);// PLL_CONTROL
    EPD_1IN54B_SendData(0x39);
    EPD_1IN54B_SendCommand(0x61);// TCON_RESOLUTION set x and y
    EPD_1IN54B_SendData(0xC8);// 200
    EPD_1IN54B_SendData(0x00);// y High eight: 0
    EPD_1IN54B_SendData(0xC8);// y Low eight: 200
    EPD_1IN54B_SendCommand(0x82);// VCM_DC_SETTING_REGISTER
    EPD_1IN54B_SendData(0x0E);

    EPD_1IN54B_SetLutBw();
    EPD_1IN54B_SetLutRed();
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_1IN54B_Clear(void)
{
    UWORD Width, Height;
    Width = (EPD_1IN54B_WIDTH % 8 == 0)? (EPD_1IN54B_WIDTH / 8 ): (EPD_1IN54B_WIDTH / 8 + 1);
    Height = EPD_1IN54B_HEIGHT;

    //send black data
    EPD_1IN54B_SendCommand(0x10);// DATA_START_TRANSMISSION_1
    DEV_Delay_ms(2);
    for(UWORD i = 0; i < Height; i++) {
        for(UWORD i = 0; i < Width; i++) {
            EPD_1IN54B_SendData(0xFF);
            EPD_1IN54B_SendData(0xFF);
        }
    }
    DEV_Delay_ms(2);

    //send red data
    EPD_1IN54B_SendCommand(0x13);// DATA_START_TRANSMISSION_2
    DEV_Delay_ms(2);
    for(UWORD i = 0; i < Height; i++) {
        for(UWORD i = 0; i < Width; i++) {
            EPD_1IN54B_SendData(0xFF);
        }
    }
    DEV_Delay_ms(2);

    EPD_1IN54B_SendCommand(0x12);// DISPLAY_REFRESH
    EPD_1IN54B_ReadBusy();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_1IN54B_Display(const UBYTE *blackimage, const UBYTE *redimage)
{
    UBYTE Temp = 0x00;
    UWORD Width, Height;
    Width = (EPD_1IN54B_WIDTH % 8 == 0)? (EPD_1IN54B_WIDTH / 8 ): (EPD_1IN54B_WIDTH / 8 + 1);
    Height = EPD_1IN54B_HEIGHT;

    EPD_1IN54B_SendCommand(0x10);// DATA_START_TRANSMISSION_1
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            Temp = 0x00;
            for (int bit = 0; bit < 4; bit++) {
                if ((blackimage[i + j * Width] & (0x80 >> bit)) != 0) {
                    Temp |= 0xC0 >> (bit * 2);
                }
            }
            EPD_1IN54B_SendData(Temp);
            Temp = 0x00;
            for (int bit = 4; bit < 8; bit++) {
                if ((blackimage[i + j * Width] & (0x80 >> bit)) != 0) {
                    Temp |= 0xC0 >> ((bit - 4) * 2);
                }
            }
            EPD_1IN54B_SendData(Temp);
        }
    }
    DEV_Delay_ms(2);

    EPD_1IN54B_SendCommand(0x13);// DATA_START_TRANSMISSION_2
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_1IN54B_SendData(redimage[i + j * Width]);
        }
    }
    DEV_Delay_ms(2);

    //Display refresh
    EPD_1IN54B_SendCommand(0x12);// DISPLAY_REFRESH
    EPD_1IN54B_ReadBusy();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_1IN54B_Sleep(void)
{
    EPD_1IN54B_SendCommand(0x50);// VCOM_AND_DATA_INTERVAL_SETTING
    EPD_1IN54B_SendData(0x17);
    EPD_1IN54B_SendCommand(0x82);// VCM_DC_SETTING_REGISTER
    EPD_1IN54B_SendData(0x00);
    EPD_1IN54B_SendCommand(0x01);// POWER_SETTING
    EPD_1IN54B_SendData(0x02);
    EPD_1IN54B_SendData(0x00);
    EPD_1IN54B_SendData(0x00);
    EPD_1IN54B_SendData(0x00);
    EPD_1IN54B_ReadBusy();
    DEV_Delay_ms(1000);
	
    EPD_1IN54B_SendCommand(0x02);// POWER_OFF
}
