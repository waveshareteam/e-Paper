/*****************************************************************************
* | File      	:   EPD_2IN13.c
* | Author      :   Waveshare team
* | Function    :   2.13inch e-paper
* | Info        :
*----------------
* |	This version:   V3.0
* | Date        :   2019-06-12
* | Info        :
* -----------------------------------------------------------------------------
* V3.0(2019-06-12):
* 1.Change:
*    EPD_Reset() => EPD_2IN13_Reset()
*    EPD_SendCommand() => EPD_2IN13_SendCommand()
*    EPD_SendData() => EPD_2IN13_SendData()
*    EPD_WaitUntilIdle() => EPD_2IN13_ReadBusy()
*    EPD_Init() => EPD_2IN13_Init()
*    EPD_Clear() => EPD_2IN13_Clear()
*    EPD_Display() => EPD_2IN13_Display()
*    EPD_Sleep() => EPD_2IN13_Sleep()
* -----------------------------------------------------------------------------
* V2.0(2019-01-03):
* 1.Remove:ImageBuff[EPD_2IN13_HEIGHT * EPD_2IN13_WIDTH / 8]
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
#include "EPD_2in13.h"
#include "Debug.h"

const unsigned char EPD_2IN13_lut_full_update[] = {
    0x22, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x11,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char EPD_2IN13_lut_partial_update[] = {
    0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_2IN13_Reset(void)
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
static void EPD_2IN13_SendCommand(UBYTE Reg)
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
static void EPD_2IN13_SendData(UBYTE Data)
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
void EPD_2IN13_ReadBusy(void)
{
    Debug("e-Paper busy\r\n");
    while(DEV_Digital_Read(EPD_BUSY_PIN) == 1) {      //LOW: idle, HIGH: busy
        DEV_Delay_ms(100);
    }
    Debug("e-Paper busy release\r\n");
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_2IN13_TurnOnDisplay(void)
{
    EPD_2IN13_SendCommand(0x22); // DISPLAY_UPDATE_CONTROL_2
    EPD_2IN13_SendData(0xC4);
    EPD_2IN13_SendCommand(0X20);	// MASTER_ACTIVATION
    EPD_2IN13_SendCommand(0xFF);	// TERMINATE_FRAME_READ_WRITE

    EPD_2IN13_ReadBusy();
}

static void EPD_2IN13_SetWindows(int x_start, int y_start, int x_end, int y_end)
{
    EPD_2IN13_SendCommand(0x44);
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    EPD_2IN13_SendData((x_start >> 3) & 0xFF);
    EPD_2IN13_SendData((x_end >> 3) & 0xFF);
    EPD_2IN13_SendCommand(0x45);
    EPD_2IN13_SendData(y_start & 0xFF);
    EPD_2IN13_SendData((y_start >> 8) & 0xFF);
    EPD_2IN13_SendData(y_end & 0xFF);
    EPD_2IN13_SendData((y_end >> 8) & 0xFF);
}

static void EPD_2IN13_SetCursor(int x, int y)
{
    EPD_2IN13_SendCommand(0x4E);
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    EPD_2IN13_SendData((x >> 3) & 0xFF);
    EPD_2IN13_SendCommand(0x4F);
    EPD_2IN13_SendData(y & 0xFF);
    EPD_2IN13_SendData((y >> 8) & 0xFF);
//    EPD_2IN13_ReadBusy();
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_2IN13_Init(UBYTE Mode)
{
    EPD_2IN13_Reset();

    EPD_2IN13_SendCommand(0x01); // DRIVER_OUTPUT_CONTROL
    EPD_2IN13_SendData((EPD_2IN13_HEIGHT - 1) & 0xFF);
    EPD_2IN13_SendData(((EPD_2IN13_HEIGHT - 1) >> 8) & 0xFF);
    EPD_2IN13_SendData(0x00);			// GD = 0; SM = 0; TB = 0;

    EPD_2IN13_SendCommand(0x0C);	// BOOSTER_SOFT_START_CONTROL
    EPD_2IN13_SendData(0xD7);
    EPD_2IN13_SendData(0xD6);
    EPD_2IN13_SendData(0x9D);

    EPD_2IN13_SendCommand(0x2C);	// WRITE_VCOM_REGISTER
    EPD_2IN13_SendData(0xA8);     // VCOM 7C

    EPD_2IN13_SendCommand(0x3A);	// SET_DUMMY_LINE_PERIOD
    EPD_2IN13_SendData(0x1A);			// 4 dummy lines per gate

    EPD_2IN13_SendCommand(0x3B);	// SET_GATE_TIME
    EPD_2IN13_SendData(0x08);			// 2us per line

    EPD_2IN13_SendCommand(0X3C);	// BORDER_WAVEFORM_CONTROL
    EPD_2IN13_SendData(0x63);

    EPD_2IN13_SendCommand(0X11);	// DATA_ENTRY_MODE_SETTING
    EPD_2IN13_SendData(0x03);			// X increment; Y increment

    //set the look-up table register
    EPD_2IN13_SendCommand(0x32);
    if(Mode == EPD_2IN13_FULL) {
        for (UWORD i = 0; i < 30; i++) {
            EPD_2IN13_SendData(EPD_2IN13_lut_full_update[i]);
        }
    } else if(Mode == EPD_2IN13_PART) {
        for (UWORD i = 0; i < 30; i++) {
            EPD_2IN13_SendData(EPD_2IN13_lut_partial_update[i]);
        }
    } else {
        Debug("error, the Mode is EPD_2IN13_FULL or EPD_2IN13_PART");
    }
}
/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_2IN13_Clear(void)
{
    UWORD Width, Height;
    Width = (EPD_2IN13_WIDTH % 8 == 0)? (EPD_2IN13_WIDTH / 8 ): (EPD_2IN13_WIDTH / 8 + 1);
    Height = EPD_2IN13_HEIGHT;

    EPD_2IN13_SetWindows(0, 0, EPD_2IN13_WIDTH, EPD_2IN13_HEIGHT);
    for (UWORD j = 0; j < Height; j++) {
        EPD_2IN13_SetCursor(0, j);
        EPD_2IN13_SendCommand(0x24);
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN13_SendData(0Xff);
        }
    }
    EPD_2IN13_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_2IN13_Display(UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_2IN13_WIDTH % 8 == 0)? (EPD_2IN13_WIDTH / 8 ): (EPD_2IN13_WIDTH / 8 + 1);
    Height = EPD_2IN13_HEIGHT;

    EPD_2IN13_SetWindows(0, 0, EPD_2IN13_WIDTH, EPD_2IN13_HEIGHT);
    for (UWORD j = 0; j < Height; j++) {
        EPD_2IN13_SetCursor(0, j);
        EPD_2IN13_SendCommand(0x24);
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN13_SendData(Image[i + j * Width]);
        }
    }
    EPD_2IN13_TurnOnDisplay();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_2IN13_Sleep(void)
{
    EPD_2IN13_SendCommand(0x10); //DEEP_SLEEP_MODE
    EPD_2IN13_SendData(0x01);
}
