/*****************************************************************************
* | File      	:   EPD_2in13d.c
* | Author      :   Waveshare team
* | Function    :   2.13inch e-paper d
* | Info        :
*----------------
* |	This version:   V3.0
* | Date        :   2019-06-12
* | Info        :
* -----------------------------------------------------------------------------
* V3.0(2019-06-12):
* 1.Change:
*    lut_vcomDC[]  => EPD_2IN13D_lut_vcomDC[]
*    lut_ww[] => EPD_2IN13D_lut_ww[]
*    lut_bw[] => EPD_2IN13D_lut_bw[]
*    lut_wb[] => EPD_2IN13D_lut_wb[]
*    lut_bb[] => EPD_2IN13D_lut_bb[]
*    lut_vcom1[] => EPD_2IN13D_lut_vcom1[]
*    lut_ww1[] => EPD_2IN13D_lut_ww1[]
*    lut_bw1[] => EPD_2IN13D_lut_bw1[]
*    lut_wb1[] => EPD_2IN13D_lut_wb1[]
*    lut_bb1[] => EPD_2IN13D_lut_bb1[]
*    EPD_Reset() => EPD_2IN13D_Reset()
*    EPD_SendCommand() => EPD_2IN13D_SendCommand()
*    EPD_SendData() => EPD_2IN13D_SendData()
*    EPD_WaitUntilIdle() => EPD_2IN13D_ReadBusy()
*    EPD_SetFullReg() => EPD_2IN13D_SetFullReg()
*    EPD_SetPartReg() => EPD_2IN13D_SetPartReg()
*    EPD_TurnOnDisplay() => EPD_2IN13D_TurnOnDisplay()
*    EPD_Init() => EPD_2IN13D_Init()
*    EPD_Clear() => EPD_2IN13D_Clear()
*    EPD_Display() => EPD_2IN13D_Display()
*    EPD_Sleep() => EPD_2IN13D_Sleep()
* V2.0(2018-11-13):
* 1.Remove:ImageBuff[EPD_2IN13D_HEIGHT * EPD_2IN13D_WIDTH / 8]
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
#include "EPD_2in13d.h"
#include "Debug.h"

/**
 * full screen update LUT
**/
static const unsigned char EPD_2IN13D_lut_vcomDC[] = {
    0x00, 0x08, 0x00, 0x00, 0x00, 0x02,
    0x60, 0x28, 0x28, 0x00, 0x00, 0x01,
    0x00, 0x14, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x12, 0x12, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,
};
static const unsigned char EPD_2IN13D_lut_ww[] = {
    0x40, 0x08, 0x00, 0x00, 0x00, 0x02,
    0x90, 0x28, 0x28, 0x00, 0x00, 0x01,
    0x40, 0x14, 0x00, 0x00, 0x00, 0x01,
    0xA0, 0x12, 0x12, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static const unsigned char EPD_2IN13D_lut_bw[] = {
    0x40, 0x17, 0x00, 0x00, 0x00, 0x02,
    0x90, 0x0F, 0x0F, 0x00, 0x00, 0x03,
    0x40, 0x0A, 0x01, 0x00, 0x00, 0x01,
    0xA0, 0x0E, 0x0E, 0x00, 0x00, 0x02,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static const unsigned char EPD_2IN13D_lut_wb[] = {
    0x80, 0x08, 0x00, 0x00, 0x00, 0x02,
    0x90, 0x28, 0x28, 0x00, 0x00, 0x01,
    0x80, 0x14, 0x00, 0x00, 0x00, 0x01,
    0x50, 0x12, 0x12, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static const unsigned char EPD_2IN13D_lut_bb[] = {
    0x80, 0x08, 0x00, 0x00, 0x00, 0x02,
    0x90, 0x28, 0x28, 0x00, 0x00, 0x01,
    0x80, 0x14, 0x00, 0x00, 0x00, 0x01,
    0x50, 0x12, 0x12, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};


/**
 * partial screen update LUT
**/
static const unsigned char EPD_2IN13D_lut_vcom1[] = {
    0x00, 0x19, 0x01, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    ,0x00, 0x00,
};
static const unsigned char EPD_2IN13D_lut_ww1[] = {
    0x00, 0x19, 0x01, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static const unsigned char EPD_2IN13D_lut_bw1[] = {
    0x80, 0x19, 0x01, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static const unsigned char EPD_2IN13D_lut_wb1[] = {
    0x40, 0x19, 0x01, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static const unsigned char EPD_2IN13D_lut_bb1[] = {
    0x00, 0x19, 0x01, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};


/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_2IN13D_Reset(void)
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
static void EPD_2IN13D_SendCommand(UBYTE Reg)
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
static void EPD_2IN13D_SendData(UBYTE Data)
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
static void EPD_2IN13D_ReadBusy(void)
{
    Debug("e-Paper busy\r\n");
    UBYTE busy;
    do {
        EPD_2IN13D_SendCommand(0x71);
        busy = DEV_Digital_Read(EPD_BUSY_PIN);
        busy =!(busy & 0x01);
    } while(busy);
    DEV_Delay_ms(200);
    Debug("e-Paper busy release\r\n");
}

/******************************************************************************
function :	LUT download
parameter:
******************************************************************************/
static void EPD_2IN13D_SetFullReg(void)
{
    EPD_2IN13D_SendCommand(0X50);			//VCOM AND DATA INTERVAL SETTING
    EPD_2IN13D_SendData(0xb7);		//WBmode:VBDF 17|D7 VBDW 97 VBDB 57		WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7

    unsigned int count;
    EPD_2IN13D_SendCommand(0x20);
    for(count=0; count<44; count++) {
        EPD_2IN13D_SendData(EPD_2IN13D_lut_vcomDC[count]);
    }

    EPD_2IN13D_SendCommand(0x21);
    for(count=0; count<42; count++) {
        EPD_2IN13D_SendData(EPD_2IN13D_lut_ww[count]);
    }

    EPD_2IN13D_SendCommand(0x22);
    for(count=0; count<42; count++) {
        EPD_2IN13D_SendData(EPD_2IN13D_lut_bw[count]);
    }

    EPD_2IN13D_SendCommand(0x23);
    for(count=0; count<42; count++) {
        EPD_2IN13D_SendData(EPD_2IN13D_lut_wb[count]);
    }

    EPD_2IN13D_SendCommand(0x24);
    for(count=0; count<42; count++) {
        EPD_2IN13D_SendData(EPD_2IN13D_lut_bb[count]);
    }
}

/******************************************************************************
function :	LUT download
parameter:
******************************************************************************/
static void EPD_2IN13D_SetPartReg(void)
{
    EPD_2IN13D_SendCommand(0x82);			//vcom_DC setting
    EPD_2IN13D_SendData(0x00);
    EPD_2IN13D_SendCommand(0X50);
    EPD_2IN13D_SendData(0xb7);
	
    unsigned int count;
    EPD_2IN13D_SendCommand(0x20);
    for(count=0; count<44; count++) {
        EPD_2IN13D_SendData(EPD_2IN13D_lut_vcom1[count]);
    }

    EPD_2IN13D_SendCommand(0x21);
    for(count=0; count<42; count++) {
        EPD_2IN13D_SendData(EPD_2IN13D_lut_ww1[count]);
    }

    EPD_2IN13D_SendCommand(0x22);
    for(count=0; count<42; count++) {
        EPD_2IN13D_SendData(EPD_2IN13D_lut_bw1[count]);
    }

    EPD_2IN13D_SendCommand(0x23);
    for(count=0; count<42; count++) {
        EPD_2IN13D_SendData(EPD_2IN13D_lut_wb1[count]);
    }

    EPD_2IN13D_SendCommand(0x24);
    for(count=0; count<42; count++) {
        EPD_2IN13D_SendData(EPD_2IN13D_lut_bb1[count]);
    }
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_2IN13D_TurnOnDisplay(void)
{
    EPD_2IN13D_SendCommand(0x12);		 //DISPLAY REFRESH
    DEV_Delay_ms(10);     //!!!The delay here is necessary, 200uS at least!!!

    EPD_2IN13D_ReadBusy();
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_2IN13D_Init()
{
    EPD_2IN13D_Reset();

    EPD_2IN13D_SendCommand(0x01);	//POWER SETTING
    EPD_2IN13D_SendData(0x03);
    EPD_2IN13D_SendData(0x00);
    EPD_2IN13D_SendData(0x2b);
    EPD_2IN13D_SendData(0x2b);
    EPD_2IN13D_SendData(0x03);

    EPD_2IN13D_SendCommand(0x06);	//boost soft start
    EPD_2IN13D_SendData(0x17);     //A
    EPD_2IN13D_SendData(0x17);     //B
    EPD_2IN13D_SendData(0x17);     //C

    EPD_2IN13D_SendCommand(0x04);
    EPD_2IN13D_ReadBusy();

    EPD_2IN13D_SendCommand(0x00);	//panel setting
    EPD_2IN13D_SendData(0xbf);     //LUT from OTP，128x296
    EPD_2IN13D_SendData(0x0e);     //VCOM to 0V fast

    EPD_2IN13D_SendCommand(0x30);	//PLL setting
    EPD_2IN13D_SendData(0x3a);     // 3a 100HZ   29 150Hz 39 200HZ	31 171HZ

    EPD_2IN13D_SendCommand(0x61);	//resolution setting
    EPD_2IN13D_SendData(EPD_2IN13D_WIDTH);
    EPD_2IN13D_SendData((EPD_2IN13D_HEIGHT >> 8) & 0xff);
    EPD_2IN13D_SendData(EPD_2IN13D_HEIGHT& 0xff);

    EPD_2IN13D_SendCommand(0x82);	//vcom_DC setting
    EPD_2IN13D_SendData(0x28);
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_2IN13D_Clear(void)
{
    UWORD Width, Height;
    Width = (EPD_2IN13D_WIDTH % 8 == 0)? (EPD_2IN13D_WIDTH / 8 ): (EPD_2IN13D_WIDTH / 8 + 1);
    Height = EPD_2IN13D_HEIGHT;

    EPD_2IN13D_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN13D_SendData(0x00);
        }
    }

    EPD_2IN13D_SendCommand(0x13);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN13D_SendData(0xFF);
        }
    }

    EPD_2IN13D_SetFullReg();
    EPD_2IN13D_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_2IN13D_Display(UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_2IN13D_WIDTH % 8 == 0)? (EPD_2IN13D_WIDTH / 8 ): (EPD_2IN13D_WIDTH / 8 + 1);
    Height = EPD_2IN13D_HEIGHT;

    EPD_2IN13D_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN13D_SendData(0x00);
        }
    }
    // Dev_Delay_ms(10);

    EPD_2IN13D_SendCommand(0x13);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN13D_SendData(Image[i + j * Width]);
        }
    }
    // Dev_Delay_ms(10);

		EPD_2IN13D_SetFullReg();
    EPD_2IN13D_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_2IN13D_DisplayPart(UBYTE *Image)
{
    /* Set partial Windows */
    EPD_2IN13D_SetPartReg();
    EPD_2IN13D_SendCommand(0x91);		//This command makes the display enter partial mode
    EPD_2IN13D_SendCommand(0x90);		//resolution setting
    EPD_2IN13D_SendData(0);           //x-start
    EPD_2IN13D_SendData(EPD_2IN13D_WIDTH - 1);       //x-end

    EPD_2IN13D_SendData(0);
    EPD_2IN13D_SendData(0);     //y-start
    EPD_2IN13D_SendData(EPD_2IN13D_HEIGHT / 256);
    EPD_2IN13D_SendData(EPD_2IN13D_HEIGHT % 256 - 1);  //y-end
    EPD_2IN13D_SendData(0x28);

    UWORD Width;
    Width = (EPD_2IN13D_WIDTH % 8 == 0)? (EPD_2IN13D_WIDTH / 8 ): (EPD_2IN13D_WIDTH / 8 + 1);
    
    /* send data */
    EPD_2IN13D_SendCommand(0x10);
    for (UWORD j = 0; j < EPD_2IN13D_HEIGHT; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN13D_SendData(~Image[i + j * Width]);
        }
    }

    EPD_2IN13D_SendCommand(0x13);
    for (UWORD j = 0; j < EPD_2IN13D_HEIGHT; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN13D_SendData(Image[i + j * Width]);
        }
    }

    /* Set partial refresh */    
    EPD_2IN13D_TurnOnDisplay();
}


/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_2IN13D_Sleep(void)
{
    EPD_2IN13D_SendCommand(0X50);
    EPD_2IN13D_SendData(0xf7);
    EPD_2IN13D_SendCommand(0X02);  	//power off
    EPD_2IN13D_SendCommand(0X07);  	//deep sleep
    EPD_2IN13D_SendData(0xA5);
}
