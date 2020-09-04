/**
 *  @filename   :   epd5in83b_V2.cpp
 *  @brief      :   Implements for e-paper library
 *  @author     :   Waveshare
 *
 *  Copyright (C) Waveshare     July 4 2020
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdlib.h>
#include "epd5in83b_V2.h"
#include "imagedata.h"

Epd::~Epd() {
};

Epd::Epd() {
    reset_pin = RST_PIN;
    dc_pin = DC_PIN;
    cs_pin = CS_PIN;
    busy_pin = BUSY_PIN;
    width = EPD_WIDTH / 8;
    height = EPD_HEIGHT;
};

int Epd::Init(void) {
  if (IfInit() != 0) {
      return -1;
  }
  Reset();

  SendCommand(0x01);      //POWER SETTING
  SendData (0x07);
  SendData (0x07);    //VGH=20V,VGL=-20V
  SendData (0x3f);    //VDH=15V
  SendData (0x3f);    //VDL=-15V

  SendCommand(0x04); //POWER ON
  DelayMs(100);  
  WaitUntilIdle();        //waiting for the electronic paper IC to release the idle signal

  SendCommand(0X00);      //PANNEL SETTING
  SendData(0x0F);   //KW-3f   KWR-2F  BWROTP 0f BWOTP 1f

  SendCommand(0x61);          //tres      
  SendData (0x02);    //source 648
  SendData (0x88);
  SendData (0x01);    //gate 480
  SendData (0xe0);

  SendCommand(0X15);    
  SendData(0x00);   

  SendCommand(0X50);      //VCOM AND DATA INTERVAL SETTING
  SendData(0x11);
  SendData(0x07);

  SendCommand(0X60);      //TCON SETTING
  SendData(0x22);
  return 0;
}

/**
 *  @brief: basic function for sending commands
 */
void Epd::SendCommand(unsigned char command) {
    DigitalWrite(dc_pin, LOW);
    SpiTransfer(command);
}

/**
 *  @brief: basic function for sending data
 */
void Epd::SendData(unsigned char data) {
    DigitalWrite(dc_pin, HIGH);
    SpiTransfer(data);
}

/**
 *  @brief: Wait until the busy_pin goes HIGH
 */
void Epd::WaitUntilIdle(void) {
  Serial.print("e-Paper busy \r\n ");
  UBYTE busy;
  do
  {
    SendCommand(0x71);
    busy = DigitalRead(busy_pin);
    busy =!(busy & 0x01);
  }
  while(busy);    
  DelayMs(200);
  Serial.print("e-Paper busy release \r\n ");
}

/**
 *  @brief: module reset.
 *          often used to awaken the module in deep sleep,
 *          see Epd::Sleep();
 */
void Epd::Reset(void) {
    DigitalWrite(reset_pin, LOW);                //module reset    
    DelayMs(1);
    DigitalWrite(reset_pin, HIGH);
    DelayMs(200);    
}

void Epd::DisplayFrame(const UBYTE *blackimage, const UBYTE *ryimage) {
    SendCommand(0x10);
    for (UWORD j = 0; j < height; j++) {
        for (UWORD i = 0; i < width; i++) {
          SendData(pgm_read_byte(&blackimage[i + (j*width)]));
        }
    }   
    SendCommand(0x13);
    for (UWORD j = 0; j < height; j++) {
        for (UWORD i = 0; i < width; i++) {
          SendData(pgm_read_byte(&ryimage[i + (j*width)]));
        }
    }
    SendCommand(0x12);
    WaitUntilIdle();
}

void Epd::DisplayPicture(const UBYTE *blackimage, UBYTE Block)
{
  UDOUBLE i, j;
  if(Block == 0){
    SendCommand(0x10);
    for (j = 0; j < height/2; j++) {
        for (i = 0; i < width; i++) {
            SendData(pgm_read_byte(&blackimage[i + j * width]));
        }
    }
    
  }else if(Block == 1){
    for (j = 0; j < height/2; j++) {
        for (i = 0; i < width; i++) {
            SendData(pgm_read_byte(&blackimage[i + j * width]));
        }
    }
  }else if(Block == 2){
    SendCommand(0x13);
    for (j = 0; j < height/2; j++) {
        for (i = 0; i < width; i++) {
            SendData(pgm_read_byte(&blackimage[i + j * width]));
        }
    }
  }else if(Block == 3){
    for (j = 0; j < height/2; j++) {
        for (i = 0; i < width; i++) {
            SendData(pgm_read_byte(&blackimage[i + j * width]));
        }
    }
    SendCommand(0x12);
    WaitUntilIdle();
  }
}

void Epd::Clear(void) {
    //send black data
    SendCommand(0x10);
    for (UWORD j = 0; j < height; j++) {
        for (UWORD i = 0; i < width; i++) {
            SendData(0xff);
        }
    }

    //send red data
    SendCommand(0x13);
    for (UWORD j = 0; j < height; j++) {
        for (UWORD i = 0; i < width; i++) {
            SendData(0x00);
        }
    }
    
    SendCommand(0x12);
    WaitUntilIdle();

}

/**
 *  @brief: After this command is transmitted, the chip would enter the 
 *          deep-sleep mode to save power. 
 *          The deep sleep mode would return to standby by hardware reset. 
 *          The only one parameter is a check code, the command would be
 *          You can use EPD_Reset() to awaken
 */
void Epd::Sleep(void) {
    SendCommand(0x02); // POWER_OFF
    WaitUntilIdle();
    SendCommand(0x07); // DEEP_SLEEP
    SendData(0xA5); // check code
}



/* END OF FILE */
