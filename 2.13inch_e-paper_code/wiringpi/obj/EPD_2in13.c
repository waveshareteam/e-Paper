/*****************************************************************************
* | File      	:		EPD_2IN13.c
* | Author      :   Waveshare team
* | Function    :   Electronic paper driver
* | Info        :
*----------------
* |	This version:   V2.0
* | Date        :   2019-01-03
* | Info        :
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
#include "EPD_2in13.h"
#include "Debug.h"

const unsigned char lut_full_update[] = {
    0x22, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x11,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char lut_partial_update[] = {
    0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_Reset(void)
{
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(200);
    DEV_Digital_Write(EPD_RST_PIN, 0);
    DEV_Delay_ms(200);
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(200);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_SendCommand(UBYTE Reg)
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
static void EPD_SendData(UBYTE Data)
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
void EPD_WaitUntilIdle(void)
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
static void EPD_TurnOnDisplay(void)
{
    EPD_SendCommand(0x22); // DISPLAY_UPDATE_CONTROL_2
    EPD_SendData(0xC4);
    EPD_SendCommand(0X20);	// MASTER_ACTIVATION
    EPD_SendCommand(0xFF);	// TERMINATE_FRAME_READ_WRITE
	
    EPD_WaitUntilIdle();
}

static void EPD_SetWindows(int x_start, int y_start, int x_end, int y_end)
{
    EPD_SendCommand(0x44);
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    EPD_SendData((x_start >> 3) & 0xFF);
    EPD_SendData((x_end >> 3) & 0xFF);
    EPD_SendCommand(0x45);
    EPD_SendData(y_start & 0xFF);
    EPD_SendData((y_start >> 8) & 0xFF);
    EPD_SendData(y_end & 0xFF);
    EPD_SendData((y_end >> 8) & 0xFF);
}

static void EPD_SetCursor(int x, int y)
{
    EPD_SendCommand(0x4E);
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    EPD_SendData((x >> 3) & 0xFF);
    EPD_SendCommand(0x4F);
    EPD_SendData(y & 0xFF);
    EPD_SendData((y >> 8) & 0xFF);
//    EPD_WaitUntilIdle();
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
UBYTE EPD_Init(const unsigned char* update)
{
    EPD_Reset();

    EPD_SendCommand(0x01); // DRIVER_OUTPUT_CONTROL
    EPD_SendData((EPD_HEIGHT - 1) & 0xFF);
    EPD_SendData(((EPD_HEIGHT - 1) >> 8) & 0xFF);
    EPD_SendData(0x00);			// GD = 0; SM = 0; TB = 0;
	
    EPD_SendCommand(0x0C);	// BOOSTER_SOFT_START_CONTROL
    EPD_SendData(0xD7);
    EPD_SendData(0xD6);
    EPD_SendData(0x9D);
	
    EPD_SendCommand(0x2C);	// WRITE_VCOM_REGISTER
    EPD_SendData(0xA8);     // VCOM 7C
	
    EPD_SendCommand(0x3A);	// SET_DUMMY_LINE_PERIOD
    EPD_SendData(0x1A);			// 4 dummy lines per gate
	
    EPD_SendCommand(0x3B);	// SET_GATE_TIME
    EPD_SendData(0x08);			// 2us per line

		EPD_SendCommand(0X3C);	// BORDER_WAVEFORM_CONTROL
		EPD_SendData(0x63);			// 2us per lin
	
    EPD_SendCommand(0X11);	// DATA_ENTRY_MODE_SETTING
    EPD_SendData(0x03);			// X increment; Y increment

		// WRITE_LUT_REGISTER
    EPD_SendCommand(0x32);	
    for (int i = 0; i < 30; i++) {
        EPD_SendData(update[i]);
    }
		
    return 0;
}
/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_Clear(void)
{
    UWORD Width, Height;
    Width = (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1);
    Height = EPD_HEIGHT;
	
		EPD_SetWindows(0, 0, EPD_WIDTH, EPD_HEIGHT);
    for (UWORD j = 0; j < Height; j++) {
				EPD_SetCursor(0, j);
				EPD_SendCommand(0x24);
        for (UWORD i = 0; i < Width; i++) {
            EPD_SendData(0Xff);
        }
    }
    EPD_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_Display(UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1);
    Height = EPD_HEIGHT;
	
		EPD_SetWindows(0, 0, EPD_WIDTH, EPD_HEIGHT);
    for (UWORD j = 0; j < Height; j++) {
				EPD_SetCursor(0, j);
				EPD_SendCommand(0x24);
        for (UWORD i = 0; i < Width; i++) {
						EPD_SendData(Image[i + j * Width]);
        }
    }
    EPD_TurnOnDisplay();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_Sleep(void)
{
    EPD_SendCommand(0x10); //DEEP_SLEEP_MODE
    EPD_SendData(0x01);
}
