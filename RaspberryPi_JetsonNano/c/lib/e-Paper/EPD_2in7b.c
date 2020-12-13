/*****************************************************************************
* | File      	:   EPD_2in7b.c
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
*    EPD_2IN7B_SendCommand() => EPD_2in7B_SendCommand()
*    EPD_2IN7B_SendData() => EPD_2in7B_SendData()
*    EPD_2IN7B_ReadBusy() => EPD_2in7B_ReadBusy()
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
* 1.Remove:ImageBuff[EPD_2IN7B_HEIGHT * EPD_2IN7B_WIDTH / 8]
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
#include "EPD_2in7b.h"
#include "Debug.h"

static const unsigned char EPD_2IN7B_lut_vcom_dc[] = {
    0x00, 0x00,
    0x00, 0x1A, 0x1A, 0x00, 0x00, 0x01,
    0x00, 0x0A, 0x0A, 0x00, 0x00, 0x08,
    0x00, 0x0E, 0x01, 0x0E, 0x01, 0x10,
    0x00, 0x0A, 0x0A, 0x00, 0x00, 0x08,
    0x00, 0x04, 0x10, 0x00, 0x00, 0x05,
    0x00, 0x03, 0x0E, 0x00, 0x00, 0x0A,
    0x00, 0x23, 0x00, 0x00, 0x00, 0x01
};

//R21H
static const unsigned char EPD_2IN7B_lut_ww[] = {
    0x90, 0x1A, 0x1A, 0x00, 0x00, 0x01,
    0x40, 0x0A, 0x0A, 0x00, 0x00, 0x08,
    0x84, 0x0E, 0x01, 0x0E, 0x01, 0x10,
    0x80, 0x0A, 0x0A, 0x00, 0x00, 0x08,
    0x00, 0x04, 0x10, 0x00, 0x00, 0x05,
    0x00, 0x03, 0x0E, 0x00, 0x00, 0x0A,
    0x00, 0x23, 0x00, 0x00, 0x00, 0x01
};

//R22H    r
static const unsigned char EPD_2IN7B_lut_bw[] = {
    0xA0, 0x1A, 0x1A, 0x00, 0x00, 0x01,
    0x00, 0x0A, 0x0A, 0x00, 0x00, 0x08,
    0x84, 0x0E, 0x01, 0x0E, 0x01, 0x10,
    0x90, 0x0A, 0x0A, 0x00, 0x00, 0x08,
    0xB0, 0x04, 0x10, 0x00, 0x00, 0x05,
    0xB0, 0x03, 0x0E, 0x00, 0x00, 0x0A,
    0xC0, 0x23, 0x00, 0x00, 0x00, 0x01
};

//R23H    w
static const unsigned char EPD_2IN7B_lut_bb[] = {
    0x90, 0x1A, 0x1A, 0x00, 0x00, 0x01,
    0x40, 0x0A, 0x0A, 0x00, 0x00, 0x08,
    0x84, 0x0E, 0x01, 0x0E, 0x01, 0x10,
    0x80, 0x0A, 0x0A, 0x00, 0x00, 0x08,
    0x00, 0x04, 0x10, 0x00, 0x00, 0x05,
    0x00, 0x03, 0x0E, 0x00, 0x00, 0x0A,
    0x00, 0x23, 0x00, 0x00, 0x00, 0x01
};

//R24H    b
static const unsigned char EPD_2IN7B_lut_wb[] = {
    0x90, 0x1A, 0x1A, 0x00, 0x00, 0x01,
    0x20, 0x0A, 0x0A, 0x00, 0x00, 0x08,
    0x84, 0x0E, 0x01, 0x0E, 0x01, 0x10,
    0x10, 0x0A, 0x0A, 0x00, 0x00, 0x08,
    0x00, 0x04, 0x10, 0x00, 0x00, 0x05,
    0x00, 0x03, 0x0E, 0x00, 0x00, 0x0A,
    0x00, 0x23, 0x00, 0x00, 0x00, 0x01
};

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_2IN7B_Reset(void)
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
static void EPD_2IN7B_SendCommand(UBYTE Reg)
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
static void EPD_2IN7B_SendData(UBYTE Data)
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
static void EPD_2IN7B_ReadBusy(void)
{
    Debug("e-Paper busy\r\n");
    while(DEV_Digital_Read(EPD_BUSY_PIN) == 0) {      //0: busy, 1: idle
        DEV_Delay_ms(100);
    }    
    Debug("e-Paper busy release\r\n");
}

/******************************************************************************
function :	set the look-up tables
parameter:
******************************************************************************/
static void EPD_2IN7B_SetLut(void)
{
    unsigned int count;     
    EPD_2IN7B_SendCommand(0x20); //vcom
    for(count = 0; count < 44; count++) {
        EPD_2IN7B_SendData(EPD_2IN7B_lut_vcom_dc[count]);
    }
    
    EPD_2IN7B_SendCommand(0x21); //ww --
    for(count = 0; count < 42; count++) {
        EPD_2IN7B_SendData(EPD_2IN7B_lut_ww[count]);
    }   
    
    EPD_2IN7B_SendCommand(0x22); //bw r
    for(count = 0; count < 42; count++) {
        EPD_2IN7B_SendData(EPD_2IN7B_lut_bw[count]);
    } 

    EPD_2IN7B_SendCommand(0x23); //wb w
    for(count = 0; count < 42; count++) {
        EPD_2IN7B_SendData(EPD_2IN7B_lut_bb[count]);
    } 

    EPD_2IN7B_SendCommand(0x24); //bb b
    for(count = 0; count < 42; count++) {
        EPD_2IN7B_SendData(EPD_2IN7B_lut_wb[count]);
    } 
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_2IN7B_Init(void)
{
    EPD_2IN7B_Reset();
    
    EPD_2IN7B_SendCommand(0x06);         //boost soft start
    EPD_2IN7B_SendData (0x07);		//A
    EPD_2IN7B_SendData (0x07);		//B
    EPD_2IN7B_SendData (0x17);		//C       

    EPD_2IN7B_SendCommand(0x04);  
    EPD_2IN7B_ReadBusy();//waiting for the electronic paper IC to release the idle signal

    EPD_2IN7B_SendCommand(0x00);			//panel setting
    EPD_2IN7B_SendData(0x0f);		//LUT from OTP￡?128x296

    EPD_2IN7B_SendCommand(0x16);
    EPD_2IN7B_SendData(0x00);				//KW-BF   KWR-AF	BWROTP 0f	

    EPD_2IN7B_SendCommand(0xF8);         //boostéè?¨
    EPD_2IN7B_SendData (0x60);
    EPD_2IN7B_SendData(0xa5);

    EPD_2IN7B_SendCommand(0xF8);         //boostéè?¨
    EPD_2IN7B_SendData (0x90);
    EPD_2IN7B_SendData (0x00);

    EPD_2IN7B_SendCommand(0xF8);         //boostéè?¨
    EPD_2IN7B_SendData (0x93);
    EPD_2IN7B_SendData(0x2A);

    EPD_2IN7B_SendCommand(0x01); // PANEL_SETTING
    EPD_2IN7B_SendData(0x03); // VDS_EN, VDG_EN
    EPD_2IN7B_SendData(0x00); // VCOM_HV, VGHL_LV[1], VGHL_LV[0]
    EPD_2IN7B_SendData(0x2b); // VDH
    EPD_2IN7B_SendData(0x2b); // VDL
    EPD_2IN7B_SendData(0x2b); // VDHR
    
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_2IN7B_Clear(void)
{    
    UWORD Width, Height;
    Width = (EPD_2IN7B_WIDTH % 8 == 0)? (EPD_2IN7B_WIDTH / 8 ): (EPD_2IN7B_WIDTH / 8 + 1);
    Height = EPD_2IN7B_HEIGHT;

    EPD_2IN7B_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN7B_SendData(0X00);
        }
    }
    EPD_2IN7B_SendCommand(0x11); // DATA_STOP

    EPD_2IN7B_SendCommand(0x13);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN7B_SendData(0X00);
        }
    }
    EPD_2IN7B_SendCommand(0x11); // DATA_STOP
    
    EPD_2IN7B_SendCommand(0x12);
    EPD_2IN7B_ReadBusy();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_2IN7B_Display(UBYTE *Imageblack, UBYTE *Imagered)
{
    UWORD Width, Height;
    Width = (EPD_2IN7B_WIDTH % 8 == 0)? (EPD_2IN7B_WIDTH / 8 ): (EPD_2IN7B_WIDTH / 8 + 1);
    Height = EPD_2IN7B_HEIGHT;

    EPD_2IN7B_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN7B_SendData(~Imageblack[i + j * Width]);
        }
    }
    EPD_2IN7B_SendCommand(0x11); // DATA_STOP
    
    EPD_2IN7B_SendCommand(0x13);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN7B_SendData(~Imagered[i + j * Width]);
        }
    }
 
    EPD_2IN7B_SendCommand(0x11); // DATA_STOP
    
    EPD_2IN7B_SendCommand(0x12);
    EPD_2IN7B_ReadBusy();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_2IN7B_Sleep(void)
{
    EPD_2IN7B_SendCommand(0X50);
    EPD_2IN7B_SendData(0xf7);
    EPD_2IN7B_SendCommand(0X02); //power off
    EPD_2IN7B_SendCommand(0X07); //deep sleep
    EPD_2IN7B_SendData(0xA5);
}
