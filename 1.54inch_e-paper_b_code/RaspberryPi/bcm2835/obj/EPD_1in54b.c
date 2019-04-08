/*****************************************************************************
* | File      	:	EPD_1in54b.c
* | Author      :   Waveshare team
* | Function    :   Electronic paper driver
* | Info        :
*----------------
* |	This version:   V2.0
* | Date        :   2018-10-30
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
#include "EPD_1in54b.h"
#include "Debug.h"


const unsigned char lut_vcom0[] = {
    0x0E, 0x14, 0x01, 0x0A, 0x06, 0x04, 0x0A, 0x0A,
    0x0F, 0x03, 0x03, 0x0C, 0x06, 0x0A, 0x00
};

const unsigned char lut_w[] = {
    0x0E, 0x14, 0x01, 0x0A, 0x46, 0x04, 0x8A, 0x4A,
    0x0F, 0x83, 0x43, 0x0C, 0x86, 0x0A, 0x04
};

const unsigned char lut_b[] = {
    0x0E, 0x14, 0x01, 0x8A, 0x06, 0x04, 0x8A, 0x4A,
    0x0F, 0x83, 0x43, 0x0C, 0x06, 0x4A, 0x04
};

const unsigned char lut_g1[] = {
    0x8E, 0x94, 0x01, 0x8A, 0x06, 0x04, 0x8A, 0x4A,
    0x0F, 0x83, 0x43, 0x0C, 0x06, 0x0A, 0x04
};

const unsigned char lut_g2[] = {
    0x8E, 0x94, 0x01, 0x8A, 0x06, 0x04, 0x8A, 0x4A,
    0x0F, 0x83, 0x43, 0x0C, 0x06, 0x0A, 0x04
};

const unsigned char lut_vcom1[] = {
    0x03, 0x1D, 0x01, 0x01, 0x08, 0x23, 0x37, 0x37,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char lut_red0[] = {
    0x83, 0x5D, 0x01, 0x81, 0x48, 0x23, 0x77, 0x77,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char lut_red1[] = {
    0x03, 0x1D, 0x01, 0x01, 0x08, 0x23, 0x37, 0x37,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
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
static void EPD_WaitUntilIdle(void)
{
    Debug("e-Paper busy\r\n");
    while(1) {      //LOW: busy, HIGH: idle
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
static void EPD_SetLutBw(void)
{
    UWORD count;
    EPD_SendCommand(0x20);         //g vcom
    for(count = 0; count < 15; count++) {
        EPD_SendData(lut_vcom0[count]);
    }
    EPD_SendCommand(0x21);        //g ww --
    for(count = 0; count < 15; count++) {
        EPD_SendData(lut_w[count]);
    }
    EPD_SendCommand(0x22);         //g bw r
    for(count = 0; count < 15; count++) {
        EPD_SendData(lut_b[count]);
    }
    EPD_SendCommand(0x23);         //g wb w
    for(count = 0; count < 15; count++) {
        EPD_SendData(lut_g1[count]);
    }
    EPD_SendCommand(0x24);         //g bb b
    for(count = 0; count < 15; count++) {
        EPD_SendData(lut_g2[count]);
    }
}

/******************************************************************************
function :	Set the look-up red tables
parameter:
******************************************************************************/
static void EPD_SetLutRed(void)
{
    UWORD count;
    EPD_SendCommand(0x25);
    for(count = 0; count < 15; count++) {
        EPD_SendData(lut_vcom1[count]);
    }
    EPD_SendCommand(0x26);
    for(count = 0; count < 15; count++) {
        EPD_SendData(lut_red0[count]);
    }
    EPD_SendCommand(0x27);
    for(count = 0; count < 15; count++) {
        EPD_SendData(lut_red1[count]);
    }
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
UBYTE EPD_Init(void)
{
    EPD_Reset();

    EPD_SendCommand(POWER_SETTING);
    EPD_SendData(0x07);
    EPD_SendData(0x00);
    EPD_SendData(0x08);
    EPD_SendData(0x00);
    EPD_SendCommand(BOOSTER_SOFT_START);
    EPD_SendData(0x07);
    EPD_SendData(0x07);
    EPD_SendData(0x07);
    EPD_SendCommand(POWER_ON);

    EPD_WaitUntilIdle();

    EPD_SendCommand(PANEL_SETTING);
    EPD_SendData(0xcf);
    EPD_SendCommand(VCOM_AND_DATA_INTERVAL_SETTING);
    EPD_SendData(0xF0);
    EPD_SendCommand(PLL_CONTROL);
    EPD_SendData(0x39);
    EPD_SendCommand(TCON_RESOLUTION);  //set x and y
    EPD_SendData(0xC8);            //x
    EPD_SendData(0x00);            //y High eight
    EPD_SendData(0xC8);            //y Low eight
    EPD_SendCommand(VCM_DC_SETTING_REGISTER); //VCOM
    EPD_SendData(0x0E);

    EPD_SetLutBw();
    EPD_SetLutRed();

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

    //send black data
    EPD_SendCommand(DATA_START_TRANSMISSION_1);
    DEV_Delay_ms(2);
    for(UWORD i = 0; i < Height; i++) {
        for(UWORD i = 0; i < Width; i++) {
            EPD_SendData(0xFF);
            EPD_SendData(0xFF);
        }
    }
    DEV_Delay_ms(2);

    //send red data
    EPD_SendCommand(DATA_START_TRANSMISSION_2);
    DEV_Delay_ms(2);
    for(UWORD i = 0; i < Height; i++) {
        for(UWORD i = 0; i < Width; i++) {
            EPD_SendData(0xFF);
        }
    }
    DEV_Delay_ms(2);

    EPD_SendCommand(DISPLAY_REFRESH);
    EPD_WaitUntilIdle();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_Display(const UBYTE *blackimage, const UBYTE *redimage)
{
    UBYTE Temp = 0x00;
    UWORD Width, Height;
    Width = (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1);
    Height = EPD_HEIGHT;

    EPD_SendCommand(DATA_START_TRANSMISSION_1);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            Temp = 0x00;
            for (int bit = 0; bit < 4; bit++) {
                if ((blackimage[i + j * Width] & (0x80 >> bit)) != 0) {
                    Temp |= 0xC0 >> (bit * 2);
                }
            }
            EPD_SendData(Temp);
            Temp = 0x00;
            for (int bit = 4; bit < 8; bit++) {
                if ((blackimage[i + j * Width] & (0x80 >> bit)) != 0) {
                    Temp |= 0xC0 >> ((bit - 4) * 2);
                }
            }
            EPD_SendData(Temp);
        }
    }
    DEV_Delay_ms(2);

    EPD_SendCommand(DATA_START_TRANSMISSION_2);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_SendData(redimage[i + j * Width]);
        }
    }
    DEV_Delay_ms(2);
    
    //Display refresh
    EPD_SendCommand(DISPLAY_REFRESH);
    EPD_WaitUntilIdle();

}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_Sleep(void)
{
    EPD_SendCommand(VCOM_AND_DATA_INTERVAL_SETTING);
    EPD_SendData(0x17);
    EPD_SendCommand(VCM_DC_SETTING_REGISTER);         //to solve Vcom drop
    EPD_SendData(0x00);
    EPD_SendCommand(POWER_SETTING);         //power setting
    EPD_SendData(0x02);        //gate switch to external
    EPD_SendData(0x00);
    EPD_SendData(0x00);
    EPD_SendData(0x00);
    EPD_WaitUntilIdle();
    EPD_SendCommand(POWER_OFF);         //power off
}
