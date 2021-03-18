/*****************************************************************************
* | File      	:   epd2in9d.h
* | Author      :   Waveshare team
* | Function    :   2.9inch e-paper
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2019-06-24
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

#include <stdlib.h>
#include "epd2in9d.h"

/**
 * partial screen update LUT
**/
const unsigned char EPD_2IN9D_lut_vcom1[] = {
    0x00, 0x19, 0x01, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    ,0x00, 0x00,
};
const unsigned char EPD_2IN9D_lut_ww1[] = {
    0x00, 0x19, 0x01, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
const unsigned char EPD_2IN9D_lut_bw1[] = {
    0x80, 0x19, 0x01, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
const unsigned char EPD_2IN9D_lut_wb1[] = {
    0x40, 0x19, 0x01, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
const unsigned char EPD_2IN9D_lut_bb1[] = {
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
 *  @brief: Wait until the busy_pin goes HIGH
 */
void Epd::WaitUntilIdle(void)
{
    char busy;
    do {
        SendCommand(0x71);
        busy = DigitalRead(busy_pin);
        busy =!(busy & 0x01);
    } while(busy);
    DelayMs(20);
}

/******************************************************************************
function :	LUT download
parameter:
******************************************************************************/
void Epd::SetPartReg(void)
{
	
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
    WaitUntilIdle();

    SendCommand(0x00);	//panel setting
    SendData(0xbf);     //LUT from OTP，128x296

    SendCommand(0x30);	//PLL setting
    SendData(0x3a);     // 3a 100HZ   29 150Hz 39 200HZ	31 171HZ

    SendCommand(0x61);	//resolution setting
    SendData(EPD_WIDTH);
    SendData((EPD_WIDTH >> 8) & 0xff);
    SendData(EPD_HEIGHT & 0xff);

    SendCommand(0x82);	//vcom_DC setting
    SendData(0x12);
	
    SendCommand(0X50);
    SendData(0x97);

    unsigned int count;
    SendCommand(0x20);
    for(count=0; count<44; count++) {
        SendData(EPD_2IN9D_lut_vcom1[count]);
    }

    SendCommand(0x21);
    for(count=0; count<42; count++) {
        SendData(EPD_2IN9D_lut_ww1[count]);
    }

    SendCommand(0x22);
    for(count=0; count<42; count++) {
        SendData(EPD_2IN9D_lut_bw1[count]);
    }

    SendCommand(0x23);
    for(count=0; count<42; count++) {
        SendData(EPD_2IN9D_lut_wb1[count]);
    }

    SendCommand(0x24);
    for(count=0; count<42; count++) {
        SendData(EPD_2IN9D_lut_bb1[count]);
    }
}

void Epd::TurnOnDisplay(void)
{
    SendCommand(0x12);		 //DISPLAY REFRESH
    DelayMs(10);     //!!!The delay here is necessary, 200uS at least!!!

    WaitUntilIdle();
}

int Epd::Init(void)
{
    /* this calls the peripheral hardware interface, see epdif */
    if (IfInit() != 0) {
        return -1;
    }

    Reset();

	SendCommand(0x04);  
	WaitUntilIdle();//waiting for the electronic paper IC to release the idle signal

	SendCommand(0x00);			//panel setting
	SendData(0x1f);		//LUT from OTP，KW-BF   KWR-AF	BWROTP 0f	BWOTP 1f

	SendCommand(0x61);			//resolution setting
	SendData (0x80);        	 
	SendData (0x01);		
	SendData (0x28);	

	SendCommand(0X50);			//VCOM AND DATA INTERVAL SETTING			
	

    return 0;
}

/**
 *  @brief: module reset.
 *          often used to awaken the module in deep sleep,
 *          see Epd::Sleep();
 */
void Epd::Reset(void)
{
    DigitalWrite(reset_pin, HIGH);
    DelayMs(20);
    DigitalWrite(reset_pin, LOW);                //module reset
    DelayMs(2);
    DigitalWrite(reset_pin, HIGH);
    DelayMs(20);
    DigitalWrite(reset_pin, LOW);                //module reset
    DelayMs(2);
    DigitalWrite(reset_pin, HIGH);
    DelayMs(20);
    DigitalWrite(reset_pin, LOW);                //module reset
    DelayMs(2);
    DigitalWrite(reset_pin, HIGH);
    DelayMs(20);
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

    TurnOnDisplay();
}

void Epd::Display(const unsigned char* frame_buffer)
{
    int w = (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1);
    int h = EPD_HEIGHT;

    SendCommand(0x10);
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            SendData(0x00);
        }
    }
    // Dev_Delay_ms(10);

    SendCommand(0x13);
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            SendData(pgm_read_byte(&frame_buffer[i + j * w]));
        }
    }
    // Dev_Delay_ms(10);

    TurnOnDisplay();
}

void Epd::DisplayPart(const unsigned char* frame_buffer)
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
    int w = (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1);
    int h = EPD_HEIGHT;

    /* send data */
    SendCommand(0x13);
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            SendData(pgm_read_byte(&frame_buffer[i + j * w]));
        }
    }

    /* Set partial refresh */
    TurnOnDisplay();
}

/**
 *  @brief: After this command is transmitted, the chip would enter the
 *          deep-sleep mode to save power.
 *          The deep sleep mode would return to standby by hardware reset.
 *          The only one parameter is a check code, the command would be
 *          executed if check code = 0xA5.
 *          You can use Epd::Init() to awaken
 */
void Epd::Sleep()
{
    SendCommand(0X50);
    SendData(0xf7);
    SendCommand(0X02);  	//power off
    WaitUntilIdle();
    SendCommand(0X07);  	//deep sleep
    SendData(0xA5);

    DigitalWrite(reset_pin, LOW);
}

/* END OF FILE */


