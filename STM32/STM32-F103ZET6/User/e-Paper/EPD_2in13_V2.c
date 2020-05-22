/*****************************************************************************
* | File      	:   EPD_2in13_V2.c
* | Author      :   Waveshare team
* | Function    :   2.13inch e-paper V2
* | Info        :
*----------------
* |	This version:   V3.0
* | Date        :   2019-06-13
* | Info        :
* -----------------------------------------------------------------------------
* V3.0(2019-06-13):
* 1.Change name:
*    EPD_Reset() => EPD_2IN13_V2_Reset()
*    EPD_SendCommand() => EPD_2IN13_V2_SendCommand()
*    EPD_SendData() => EPD_2IN13_V2_SendData()
*    EPD_WaitUntilIdle() => EPD_2IN13_V2_ReadBusy()
*    EPD_Init() => EPD_2IN13_V2_Init()
*    EPD_Clear() => EPD_2IN13_V2_Clear()
*    EPD_Display() => EPD_2IN13_V2_Display()
*    EPD_Sleep() => EPD_2IN13_V2_Sleep()
* 2.add:
*    EPD_2IN13_V2_DisplayPartBaseImage()
* -----------------------------------------------------------------------------
* V2.0(2018-11-14):
* 1.Remove:ImageBuff[EPD_HEIGHT * EPD_WIDTH / 8]
* 2.Change:EPD_2IN13_V2_Display(UBYTE *Image)
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
#include "EPD_2in13_V2.h"
#include "Debug.h"

const unsigned char EPD_2IN13_V2_lut_full_update[]= {
    0x80,0x60,0x40,0x00,0x00,0x00,0x00,             //LUT0: BB:     VS 0 ~7
    0x10,0x60,0x20,0x00,0x00,0x00,0x00,             //LUT1: BW:     VS 0 ~7
    0x80,0x60,0x40,0x00,0x00,0x00,0x00,             //LUT2: WB:     VS 0 ~7
    0x10,0x60,0x20,0x00,0x00,0x00,0x00,             //LUT3: WW:     VS 0 ~7
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,             //LUT4: VCOM:   VS 0 ~7

    0x03,0x03,0x00,0x00,0x02,                       // TP0 A~D RP0
    0x09,0x09,0x00,0x00,0x02,                       // TP1 A~D RP1
    0x03,0x03,0x00,0x00,0x02,                       // TP2 A~D RP2
    0x00,0x00,0x00,0x00,0x00,                       // TP3 A~D RP3
    0x00,0x00,0x00,0x00,0x00,                       // TP4 A~D RP4
    0x00,0x00,0x00,0x00,0x00,                       // TP5 A~D RP5
    0x00,0x00,0x00,0x00,0x00,                       // TP6 A~D RP6

    0x15,0x41,0xA8,0x32,0x30,0x0A,
};

const unsigned char EPD_2IN13_V2_lut_partial_update[]= { //20 bytes
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,             //LUT0: BB:     VS 0 ~7
    0x80,0x00,0x00,0x00,0x00,0x00,0x00,             //LUT1: BW:     VS 0 ~7
    0x40,0x00,0x00,0x00,0x00,0x00,0x00,             //LUT2: WB:     VS 0 ~7
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,             //LUT3: WW:     VS 0 ~7
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,             //LUT4: VCOM:   VS 0 ~7

    0x0A,0x00,0x00,0x00,0x00,                       // TP0 A~D RP0
    0x00,0x00,0x00,0x00,0x00,                       // TP1 A~D RP1
    0x00,0x00,0x00,0x00,0x00,                       // TP2 A~D RP2
    0x00,0x00,0x00,0x00,0x00,                       // TP3 A~D RP3
    0x00,0x00,0x00,0x00,0x00,                       // TP4 A~D RP4
    0x00,0x00,0x00,0x00,0x00,                       // TP5 A~D RP5
    0x00,0x00,0x00,0x00,0x00,                       // TP6 A~D RP6

    0x15,0x41,0xA8,0x32,0x30,0x0A,
};
/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_2IN13_V2_Reset(void)
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
static void EPD_2IN13_V2_SendCommand(UBYTE Reg)
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
static void EPD_2IN13_V2_SendData(UBYTE Data)
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
void EPD_2IN13_V2_ReadBusy(void)
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
static void EPD_2IN13_V2_TurnOnDisplay(void)
{
    EPD_2IN13_V2_SendCommand(0x22);
    EPD_2IN13_V2_SendData(0xC7);
    EPD_2IN13_V2_SendCommand(0x20);
    EPD_2IN13_V2_ReadBusy();
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_2IN13_V2_TurnOnDisplayPart(void)
{
    EPD_2IN13_V2_SendCommand(0x22);
    EPD_2IN13_V2_SendData(0x0C);
    EPD_2IN13_V2_SendCommand(0x20);
    EPD_2IN13_V2_ReadBusy();
}
/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_2IN13_V2_Init(UBYTE Mode)
{
    UBYTE count;
    EPD_2IN13_V2_Reset();

    if(Mode == EPD_2IN13_V2_FULL) {
        EPD_2IN13_V2_ReadBusy();
        EPD_2IN13_V2_SendCommand(0x12); // soft reset
        EPD_2IN13_V2_ReadBusy();

        EPD_2IN13_V2_SendCommand(0x74); //set analog block control
        EPD_2IN13_V2_SendData(0x54);
        EPD_2IN13_V2_SendCommand(0x7E); //set digital block control
        EPD_2IN13_V2_SendData(0x3B);

        EPD_2IN13_V2_SendCommand(0x01); //Driver output control
        EPD_2IN13_V2_SendData(0xF9);
        EPD_2IN13_V2_SendData(0x00);
        EPD_2IN13_V2_SendData(0x00);

        EPD_2IN13_V2_SendCommand(0x11); //data entry mode
        EPD_2IN13_V2_SendData(0x01);

        EPD_2IN13_V2_SendCommand(0x44); //set Ram-X address start/end position
        EPD_2IN13_V2_SendData(0x00);
        EPD_2IN13_V2_SendData(0x0F);    //0x0C-->(15+1)*8=128

        EPD_2IN13_V2_SendCommand(0x45); //set Ram-Y address start/end position
        EPD_2IN13_V2_SendData(0xF9);   //0xF9-->(249+1)=250
        EPD_2IN13_V2_SendData(0x00);
        EPD_2IN13_V2_SendData(0x00);
        EPD_2IN13_V2_SendData(0x00);

        EPD_2IN13_V2_SendCommand(0x3C); //BorderWavefrom
        EPD_2IN13_V2_SendData(0x03);

        EPD_2IN13_V2_SendCommand(0x2C); //VCOM Voltage
        EPD_2IN13_V2_SendData(0x55); //

        EPD_2IN13_V2_SendCommand(0x03);
        EPD_2IN13_V2_SendData(EPD_2IN13_V2_lut_full_update[70]);

        EPD_2IN13_V2_SendCommand(0x04); //
        EPD_2IN13_V2_SendData(EPD_2IN13_V2_lut_full_update[71]);
        EPD_2IN13_V2_SendData(EPD_2IN13_V2_lut_full_update[72]);
        EPD_2IN13_V2_SendData(EPD_2IN13_V2_lut_full_update[73]);

        EPD_2IN13_V2_SendCommand(0x3A);     //Dummy Line
        EPD_2IN13_V2_SendData(EPD_2IN13_V2_lut_full_update[74]);
        EPD_2IN13_V2_SendCommand(0x3B);     //Gate time
        EPD_2IN13_V2_SendData(EPD_2IN13_V2_lut_full_update[75]);

        EPD_2IN13_V2_SendCommand(0x32);
        for(count = 0; count < 70; count++) {
            EPD_2IN13_V2_SendData(EPD_2IN13_V2_lut_full_update[count]);
        }

        EPD_2IN13_V2_SendCommand(0x4E);   // set RAM x address count to 0;
        EPD_2IN13_V2_SendData(0x00);
        EPD_2IN13_V2_SendCommand(0x4F);   // set RAM y address count to 0X127;
        EPD_2IN13_V2_SendData(0xF9);
        EPD_2IN13_V2_SendData(0x00);
        EPD_2IN13_V2_ReadBusy();
    } else if(Mode == EPD_2IN13_V2_PART) {
        EPD_2IN13_V2_SendCommand(0x2C);     //VCOM Voltage
        EPD_2IN13_V2_SendData(0x26);

        EPD_2IN13_V2_ReadBusy();

        EPD_2IN13_V2_SendCommand(0x32);
        for(count = 0; count < 70; count++) {
            EPD_2IN13_V2_SendData(EPD_2IN13_V2_lut_partial_update[count]);
        }

        EPD_2IN13_V2_SendCommand(0x37);
        EPD_2IN13_V2_SendData(0x00);
        EPD_2IN13_V2_SendData(0x00);
        EPD_2IN13_V2_SendData(0x00);
        EPD_2IN13_V2_SendData(0x00);
        EPD_2IN13_V2_SendData(0x40);
        EPD_2IN13_V2_SendData(0x00);
        EPD_2IN13_V2_SendData(0x00);

        EPD_2IN13_V2_SendCommand(0x22);
        EPD_2IN13_V2_SendData(0xC0);

        EPD_2IN13_V2_SendCommand(0x20);
        EPD_2IN13_V2_ReadBusy();

        EPD_2IN13_V2_SendCommand(0x3C); //BorderWavefrom
        EPD_2IN13_V2_SendData(0x01);
    } else {
        Debug("error, the Mode is EPD_2IN13_FULL or EPD_2IN13_PART");
    }
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_2IN13_V2_Clear(void)
{
    UWORD Width, Height;
    Width = (EPD_2IN13_V2_WIDTH % 8 == 0)? (EPD_2IN13_V2_WIDTH / 8 ): (EPD_2IN13_V2_WIDTH / 8 + 1);
    Height = EPD_2IN13_V2_HEIGHT;

    EPD_2IN13_V2_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN13_V2_SendData(0XFF);
        }
    }

    EPD_2IN13_V2_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_2IN13_V2_Display(UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_2IN13_V2_WIDTH % 8 == 0)? (EPD_2IN13_V2_WIDTH / 8 ): (EPD_2IN13_V2_WIDTH / 8 + 1);
    Height = EPD_2IN13_V2_HEIGHT;

    EPD_2IN13_V2_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN13_V2_SendData(Image[i + j * Width]);
        }
    }
    EPD_2IN13_V2_TurnOnDisplay();
}

/******************************************************************************
function :	 The image of the previous frame must be uploaded, otherwise the
		         first few seconds will display an exception.
parameter:
******************************************************************************/
void EPD_2IN13_V2_DisplayPartBaseImage(UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_2IN13_V2_WIDTH % 8 == 0)? (EPD_2IN13_V2_WIDTH / 8 ): (EPD_2IN13_V2_WIDTH / 8 + 1);
    Height = EPD_2IN13_V2_HEIGHT;

    UDOUBLE Addr = 0;
    EPD_2IN13_V2_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            Addr = i + j * Width;
            EPD_2IN13_V2_SendData(Image[Addr]);
        }
    }
    EPD_2IN13_V2_SendCommand(0x26);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            Addr = i + j * Width;
            EPD_2IN13_V2_SendData(Image[Addr]);
        }
    }
    EPD_2IN13_V2_TurnOnDisplay();
}


void EPD_2IN13_V2_DisplayPart(UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_2IN13_V2_WIDTH % 8 == 0)? (EPD_2IN13_V2_WIDTH / 8 ): (EPD_2IN13_V2_WIDTH / 8 + 1);
    Height = EPD_2IN13_V2_HEIGHT;
    EPD_2IN13_V2_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN13_V2_SendData(Image[i + j * Width]);
        }
    }

    EPD_2IN13_V2_TurnOnDisplayPart();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_2IN13_V2_Sleep(void)
{
    EPD_2IN13_V2_SendCommand(0x22); //POWER OFF
    EPD_2IN13_V2_SendData(0xC3);
    EPD_2IN13_V2_SendCommand(0x20);

    EPD_2IN13_V2_SendCommand(0x10); //enter deep sleep
    EPD_2IN13_V2_SendData(0x01);
    DEV_Delay_ms(100);
}
