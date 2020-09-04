/*****************************************************************************
* | File      	:  	EPD_2in9.c
* | Author      :   Waveshare team
* | Function    :   2.9inch e-paper
* | Info        :
*----------------
* |	This version:   V3.0
* | Date        :   2019-06-12
* | Info        :
* -----------------------------------------------------------------------------
* V3.0(2019-06-12):
* 1.Change:
*    lut_full_update[] => EPD_2IN9_lut_full_update[]
*    lut_partial_update[] => EPD_2IN9_lut_partial_update[] 
*    EPD_Reset() => EPD_2IN9_Reset()
*    EPD_SendCommand() => EPD_2IN9_SendCommand()
*    EPD_SendData() => EPD_2IN9_SendData()
*    EPD_WaitUntilIdle() => EPD_2IN9_ReadBusy()
*    EPD_SetLut() => EPD_2IN9_SetLut()
*    EPD_SetWindow() => EPD_2IN9_SetWindow()
*    EPD_SetCursor() => EPD_2IN9_SetCursor()
*    EPD_TurnOnDisplay() => EPD_2IN9_TurnOnDisplay()
*    EPD_Init() => EPD_2IN9_Init()
*    EPD_Clear() => EPD_2IN9_Clear()
*    EPD_Display() => EPD_2IN9_Display()
*    EPD_Sleep() => EPD_2IN9_Sleep()
* 2.remove commands define:
*    #define DRIVER_OUTPUT_CONTROL                       0x01
*    #define BOOSTER_SOFT_START_CONTROL                  0x0C
*    #define GATE_SCAN_START_POSITION                    0x0F
*    #define DEEP_SLEEP_MODE                             0x10
*    #define DATA_ENTRY_MODE_SETTING                     0x11
*    #define SW_RESET                                    0x12
*    #define TEMPERATURE_SENSOR_CONTROL                  0x1A
*    #define MASTER_ACTIVATION                           0x20
*    #define DISPLAY_UPDATE_CONTROL_1                    0x21
*    #define DISPLAY_UPDATE_CONTROL_2                    0x22
*    #define WRITE_RAM                                   0x24
*    #define WRITE_VCOM_REGISTER                         0x2C
*    #define WRITE_LUT_REGISTER                          0x32
*    #define SET_DUMMY_LINE_PERIOD                       0x3A
*    #define SET_GATE_TIME                               0x3B
*    #define BORDER_WAVEFORM_CONTROL                     0x3C
*    #define SET_RAM_X_ADDRESS_START_END_POSITION        0x44
*    #define SET_RAM_Y_ADDRESS_START_END_POSITION        0x45
*    #define SET_RAM_X_ADDRESS_COUNTER                   0x4E
*    #define SET_RAM_Y_ADDRESS_COUNTER                   0x4F
*    #define TERMINATE_FRAME_READ_WRITE                  0xFF
* -----------------------------------------------------------------------------
* V2.0(2018-11-06):
* 1.Remove:ImageBuff[EPD_2IN9_HEIGHT * EPD_2IN9_WIDTH / 8]
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
#include "EPD_2in9.h"
#include "Debug.h"

const unsigned char EPD_2IN9_lut_full_update[] = {
    0x50, 0xAA, 0x55, 0xAA, 0x11, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xFF, 0xFF, 0x1F, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char EPD_2IN9_lut_partial_update[] = {
    0x10, 0x18, 0x18, 0x08, 0x18, 0x18,
    0x08, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x13, 0x14, 0x44, 0x12,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_2IN9_Reset(void)
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
static void EPD_2IN9_SendCommand(UBYTE Reg)
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
static void EPD_2IN9_SendData(UBYTE Data)
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
void EPD_2IN9_ReadBusy(void)
{
    Debug("e-Paper busy\r\n");
    while(DEV_Digital_Read(EPD_BUSY_PIN) == 1) {      //LOW: idle, HIGH: busy
        DEV_Delay_ms(100);
    }
    Debug("e-Paper busy release\r\n");
}

/******************************************************************************
function :	Setting the display window
parameter:
******************************************************************************/
static void EPD_2IN9_SetWindows(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend)
{
    EPD_2IN9_SendCommand(0x44); // SET_RAM_X_ADDRESS_START_END_POSITION
    EPD_2IN9_SendData((Xstart >> 3) & 0xFF);
    EPD_2IN9_SendData((Xend >> 3) & 0xFF);

    EPD_2IN9_SendCommand(0x45); // SET_RAM_Y_ADDRESS_START_END_POSITION
    EPD_2IN9_SendData(Ystart & 0xFF);
    EPD_2IN9_SendData((Ystart >> 8) & 0xFF);
    EPD_2IN9_SendData(Yend & 0xFF);
    EPD_2IN9_SendData((Yend >> 8) & 0xFF);
}

/******************************************************************************
function :	Set Cursor
parameter:
******************************************************************************/
static void EPD_2IN9_SetCursor(UWORD Xstart, UWORD Ystart)
{
    EPD_2IN9_SendCommand(0x4E); // SET_RAM_X_ADDRESS_COUNTER
    EPD_2IN9_SendData((Xstart >> 3) & 0xFF);

    EPD_2IN9_SendCommand(0x4F); // SET_RAM_Y_ADDRESS_COUNTER
    EPD_2IN9_SendData(Ystart & 0xFF);
    EPD_2IN9_SendData((Ystart >> 8) & 0xFF);
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_2IN9_TurnOnDisplay(void)
{
    EPD_2IN9_SendCommand(0x22); // DISPLAY_UPDATE_CONTROL_2
    EPD_2IN9_SendData(0xC4);
    EPD_2IN9_SendCommand(0x20); // MASTER_ACTIVATION
    EPD_2IN9_SendCommand(0xFF); // TERMINATE_FRAME_READ_WRITE

    EPD_2IN9_ReadBusy();
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_2IN9_Init(UBYTE Mode)
{
    EPD_2IN9_Reset();

    EPD_2IN9_SendCommand(0x01); // DRIVER_OUTPUT_CONTROL
    EPD_2IN9_SendData((EPD_2IN9_HEIGHT - 1) & 0xFF);
    EPD_2IN9_SendData(((EPD_2IN9_HEIGHT - 1) >> 8) & 0xFF);
    EPD_2IN9_SendData(0x00); // GD = 0; SM = 0; TB = 0;
	
    EPD_2IN9_SendCommand(0x0C); // BOOSTER_SOFT_START_CONTROL
    EPD_2IN9_SendData(0xD7);
    EPD_2IN9_SendData(0xD6);
    EPD_2IN9_SendData(0x9D);
	
    EPD_2IN9_SendCommand(0x2C); // WRITE_VCOM_REGISTER
    EPD_2IN9_SendData(0xA8); // VCOM 7C
	
    EPD_2IN9_SendCommand(0x3A); // SET_DUMMY_LINE_PERIOD
    EPD_2IN9_SendData(0x1A); // 4 dummy lines per gate
	
    EPD_2IN9_SendCommand(0x3B); // SET_GATE_TIME
    EPD_2IN9_SendData(0x08); // 2us per line
	
    EPD_2IN9_SendCommand(0x3C); // BORDER_WAVEFORM_CONTROL
    EPD_2IN9_SendData(0x03);                     
    EPD_2IN9_SendCommand(0x11); // DATA_ENTRY_MODE_SETTING
    EPD_2IN9_SendData(0x03);

    //set the look-up table register
    EPD_2IN9_SendCommand(0x32); // WRITE_LUT_REGISTER
    if(Mode == EPD_2IN9_FULL){
        for (UWORD i = 0; i < 30; i++) {
                EPD_2IN9_SendData(EPD_2IN9_lut_full_update[i]);
        }
    }else if(Mode == EPD_2IN9_PART){
        for (UWORD i = 0; i < 30; i++) {
                EPD_2IN9_SendData(EPD_2IN9_lut_partial_update[i]);
        }
    }else{
        Debug("error, the Mode is EPD_2IN9_FULL or EPD_2IN9_PART");
    }
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_2IN9_Clear(void)
{
    UWORD Width, Height;
    Width = (EPD_2IN9_WIDTH % 8 == 0)? (EPD_2IN9_WIDTH / 8 ): (EPD_2IN9_WIDTH / 8 + 1);
    Height = EPD_2IN9_HEIGHT;
    EPD_2IN9_SetWindows(0, 0, EPD_2IN9_WIDTH, EPD_2IN9_HEIGHT);
    for (UWORD j = 0; j < Height; j++) {
        EPD_2IN9_SetCursor(0, j);
        EPD_2IN9_SendCommand(0x24);
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN9_SendData(0XFF);
        }
    }
    EPD_2IN9_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_2IN9_Display(UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_2IN9_WIDTH % 8 == 0)? (EPD_2IN9_WIDTH / 8 ): (EPD_2IN9_WIDTH / 8 + 1);
    Height = EPD_2IN9_HEIGHT;

    UDOUBLE Addr = 0;
    // UDOUBLE Offset = ImageName;
    EPD_2IN9_SetWindows(0, 0, EPD_2IN9_WIDTH, EPD_2IN9_HEIGHT);
    for (UWORD j = 0; j < Height; j++) {
        EPD_2IN9_SetCursor(0, j);
        EPD_2IN9_SendCommand(0x24);
        for (UWORD i = 0; i < Width; i++) {
            Addr = i + j * Width;
            EPD_2IN9_SendData(Image[Addr]);
        }
    }
    EPD_2IN9_TurnOnDisplay();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_2IN9_Sleep(void)
{
    EPD_2IN9_SendCommand(0x10);
    EPD_2IN9_SendData(0x01);
    // EPD_2IN9_ReadBusy();
}
