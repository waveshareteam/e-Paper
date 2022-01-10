/*****************************************************************************
* | File      	:   EPD_2in13d
* | Author      :   Waveshare team
* | Function    :   2.13inch e-paper d
* | Info        :
*----------------
* |	This version:   V4.0
* | Date        :   2019-12-31
* | Info        :
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
#include "epd2in13d.h"
#include <arduino.h>
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

Epd::~Epd()
{
};

Epd::Epd()
{
    reset_pin = RST_PIN;
    dc_pin = DC_PIN;
    cs_pin = CS_PIN;
    busy_pin = BUSY_PIN;
    width = EPD_WIDTH;
    height = EPD_HEIGHT;
};

/**
 *  @brief: basic function for sending commands
 */
void Epd::SendCommand(unsigned char command)
{
    DigitalWrite(dc_pin, LOW);
    SpiTransfer(command);
}

/**
 *  @brief: basic function for sending data
 */
void Epd::SendData(unsigned char data)
{
    DigitalWrite(dc_pin, HIGH);
    SpiTransfer(data);
}

/**
 *  @brief: module reset.
 *          often used to awaken the module in deep sleep,
 *          see Epd::Sleep();
 */
void Epd::Reset(void)
{
    DigitalWrite(reset_pin, HIGH);
    DelayMs(200);
    DigitalWrite(reset_pin, LOW);                //module reset
    DelayMs(10);
    DigitalWrite(reset_pin, HIGH);
    DelayMs(200);
}

/******************************************************************************
function :	Wait until the busy_pin goes LOW
parameter:
******************************************************************************/
void Epd::ReadBusy(void)
{
    Serial.print("ReadBusy\n");
    char busy;
    do {
        SendCommand(0x71);
        busy = DigitalRead(busy_pin);
        busy =!(busy & 0x01);
    } while(busy);
    Serial.print("ReadBusy over\n");
    DelayMs(200);
}

/******************************************************************************
function :	LUT download
parameter:
******************************************************************************/
void Epd::SetFullReg(void)
{
    SendCommand(0x82);
    SendData(0x00);	
    SendCommand(0X50);
    SendData(0x97);

    unsigned int count;
    SendCommand(0x20);
    for(count=0; count<44; count++) {
        SendData(EPD_2IN13D_lut_vcomDC[count]);
    }

    SendCommand(0x21);
    for(count=0; count<42; count++) {
        SendData(EPD_2IN13D_lut_ww[count]);
    }

    SendCommand(0x22);
    for(count=0; count<42; count++) {
        SendData(EPD_2IN13D_lut_bw[count]);
    }

    SendCommand(0x23);
    for(count=0; count<42; count++) {
        SendData(EPD_2IN13D_lut_wb[count]);
    }

    SendCommand(0x24);
    for(count=0; count<42; count++) {
        SendData(EPD_2IN13D_lut_bb[count]);
    }
}

/******************************************************************************
function :	LUT download
parameter:
******************************************************************************/
void Epd::SetPartReg(void)
{
    SendCommand(0x82);			//vcom_DC setting
    SendData(0x03);
    SendCommand(0X50);
    SendData(0x47);
	
    unsigned int count;
    SendCommand(0x20);
    for(count=0; count<44; count++) {
        SendData(EPD_2IN13D_lut_vcom1[count]);
    }

    SendCommand(0x21);
    for(count=0; count<42; count++) {
        SendData(EPD_2IN13D_lut_ww1[count]);
    }

    SendCommand(0x22);
    for(count=0; count<42; count++) {
        SendData(EPD_2IN13D_lut_bw1[count]);
    }

    SendCommand(0x23);
    for(count=0; count<42; count++) {
        SendData(EPD_2IN13D_lut_wb1[count]);
    }

    SendCommand(0x24);
    for(count=0; count<42; count++) {
        SendData(EPD_2IN13D_lut_bb1[count]);
    }
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
void Epd::TurnOnDisplay(void)
{
    SendCommand(0x12);		 //DISPLAY REFRESH
    DelayMs(10);     //!!!The delay here is necessary, 200uS at least!!!

    ReadBusy();
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
int Epd::Init()
{
    /* this calls the peripheral hardware interface, see epdif */
    if (IfInit() != 0) {
        return -1;
    }

    Reset();
    SendCommand(0x01);	//POWER SETTING
    SendData(0x03);
    SendData(0x00);
    SendData(0x2b);
    SendData(0x2b);
    SendData(0x03);

    SendCommand(0x06);	//boost soft start
    SendData(0x17);     //A
    SendData(0x17);     //B
    SendData(0x17);     //C

    SendCommand(0x04);
    ReadBusy();

    SendCommand(0x00);	//panel setting
    SendData(0xbf);     //LUT from OTP，128x296
    SendData(0x0e);     //VCOM to 0V fast

    SendCommand(0x30);	//PLL setting
    SendData(0x3a);     // 3a 100HZ   29 150Hz 39 200HZ	31 171HZ

    SendCommand(0x61);	//resolution setting
    SendData(EPD_WIDTH);
    SendData((EPD_HEIGHT >> 8) & 0xff);
    SendData(EPD_HEIGHT& 0xff);

    SendCommand(0x82);	//vcom_DC setting
    SendData(0x28);
    
    return 0;
}

void Epd::Clear(void)
{
    int w, h;
    w = (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1);
    h = EPD_HEIGHT;

    SendCommand(0x10);
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            SendData(0x00);
        }
    }

    SendCommand(0x13);
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            SendData(0xFF);
        }
    }

    SetFullReg();
    TurnOnDisplay();
}


/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void Epd::Display(const unsigned char *Image)
{
    int Width, Height;
    Width = (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1);
    Height = EPD_HEIGHT;

    SendCommand(0x10);
    for (int j = 0; j < Height; j++) {
        for (int i = 0; i < Width; i++) {
            SendData(0x00);
        }
    }

    SendCommand(0x13);
    for (int j = 0; j < Height; j++) {
        for (int i = 0; i < Width; i++) {
            SendData(Image[i + j * Width]);
        }
    }

	SetFullReg();
    TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void Epd::Displayimage(const unsigned char *Image)
{
    int Width, Height;
    Width = (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1);
    Height = EPD_HEIGHT;

    SendCommand(0x10);
    for (int j = 0; j < Height; j++) {
        for (int i = 0; i < Width; i++) {
            SendData(0x00);
        }
    }

    SendCommand(0x13);
    for (int j = 0; j < Height; j++) {
        for (int i = 0; i < Width; i++) {
            SendData(pgm_read_byte(&Image[i + j * Width]));
        }
    }

	SetFullReg();
    TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void Epd::DisplayPart(const unsigned char *Image)
{
    /* Set partial Windows */
    SetPartReg();
    SendCommand(0x91);		//This command makes the display enter partial mode
    SendCommand(0x90);		//resolution setting
    SendData(0);           //x-start
    SendData(EPD_WIDTH - 1);       //x-end

    SendData(0);
    SendData(0);     //y-start
    SendData(EPD_HEIGHT / 256);
    SendData(EPD_HEIGHT % 256 - 1);  //y-end
    SendData(0x28);

    int Width = (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1);
    
    /* send data */
    SendCommand(0x10);
    for (int j = 0; j < EPD_HEIGHT; j++) {
        for (int i = 0; i < Width; i++) {
            SendData(~Image[i + j * Width]);
        }
    }

    SendCommand(0x13);
    for (int j = 0; j < EPD_HEIGHT; j++) {
        for (int i = 0; i < Width; i++) {
            SendData(Image[i + j * Width]);
        }
    }

    /* Set partial refresh */    
    TurnOnDisplay();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void Epd::Sleep(void)
{
    SendCommand(0X50);
    SendData(0xf7);
    SendCommand(0X02);  	//power off
    SendCommand(0X07);  	//deep sleep
    SendData(0xA5);

    DigitalWrite(reset_pin, LOW);
}
