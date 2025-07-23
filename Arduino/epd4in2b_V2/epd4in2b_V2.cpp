/**
 *  @filename   :   epd4in2b_V2.cpp
 *  @brief      :   Implements for Dual-color e-paper library
 *  @author     :	Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     Nov 25 2020
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
#include "epd4in2b_V2.h"

Epd::~Epd() {
};

Epd::Epd() {
    flag=0;
    reset_pin = RST_PIN;
    dc_pin = DC_PIN;
    cs_pin = CS_PIN;
    busy_pin = BUSY_PIN;
    width = EPD_WIDTH;
    height = EPD_HEIGHT;
};

int Epd::Init_old(void) {
    Reset();
    SendCommand(0x04);
    ReadBusy();
	
    SendCommand(0x00);
    SendData(0x0F);     // LUT from OTP
	
    // /* EPD hardware init end */
    return 0;
}

int Epd::Init_new(void) {

    Reset();

    ReadBusy();
    SendCommand(0x12);
    ReadBusy();

    SendCommand(0x3C); //BorderWavefrom
    SendData(0x05);	

    SendCommand(0x18); //Read built-in temperature sensor
    SendData(0x80);	

    SendCommand(0x11); //data entry mode       
    SendData(0x03);

    SendCommand(0x44); //set Ram-X address start/end position   
    SendData(0x00);
    SendData(width/8-1);

    SendCommand(0x45); //set Ram-Y address start/end position          
    SendData(0x00);
    SendData(0x00); 
    SendData((height-1)%256);    
    SendData((height-1)/256);

    SendCommand(0x4E);   // set RAM x address count to 0;
    SendData(0x00);
    SendCommand(0x4F);   // set RAM y address count to 0X199;    
    SendData(0x00);    
    SendData(0x00);
    ReadBusy();

    return 0;
}

int Epd::Init(void) {
    unsigned char i=0;

    /* this calls the peripheral hardware interface, see epdif */
    if (IfInit() != 0) {
        return -1;
    }
    /* EPD hardware init start */
    Reset();

    EPD_GPIO_Init();
    digitalWrite(dc_pin, 0);
    EPD_SendData(0x2F);
    DelayMs(50);
    
    digitalWrite(dc_pin, 1);
    i = EPD_ReadData();
    printf("%02x\n",i);
    
    EPD_SPI_Init();
    if(i == 0x01)
    {
        flag = 0;
        Init_new();
    }
    else
    {
        flag = 1;
        Init_old();
    }
	
    // /* EPD hardware init end */
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
void Epd::ReadBusy(void) {
    if(flag == 0)
    {
        while(DigitalRead(busy_pin) == 1) {
            DelayMs(100);
        } 
    }
    else
    {
        while(DigitalRead(busy_pin) == 0) {
            DelayMs(100);
        } 
    }     
}

/**
 *  @brief: module reset. 
 *          often used to awaken the module in deep sleep, 
 *          see Epd::Sleep();
 */
void Epd::Reset(void) {
    DigitalWrite(reset_pin, HIGH);
    DelayMs(200);   
    DigitalWrite(reset_pin, LOW);
    DelayMs(2);
    DigitalWrite(reset_pin, HIGH);
    DelayMs(200);   
}

/**
 *  @brief: transmit partial data to the black part of SRAM
 */
void Epd::Display_Window_Black(const UBYTE *image, UBYTE count) {
    UBYTE k;
    if(count == 0 && flag == 0)
        SendCommand(0x24);
    else if(count == 0)
        SendCommand(0x10);
    
    for (UWORD j = 0; j < 100; j++) {
        for (UWORD i = 0; i < 50; i++) {
            if(i<16)
            {
                SendData(image[i + (j*16)]);
            }
            else
            {
                SendData(0xFF);
            } 
        }
    }
}

/**
 *  @brief: transmit partial data to the red part of SRAM
 */
void Epd::Display_Window_Red_new(const UBYTE *image, UBYTE count) {
    UBYTE k;
    if(count == 0)
        SendCommand(0x26);
    
    for (UWORD j = 0; j < 100; j++) {
        for (UWORD i = 0; i < 50; i++) {
            if(i<16)
            {
                SendData(~image[i + (j*16)]);
            }
            else
            {
                SendData(0x00);
            } 
        }
    }
}
void Epd::Display_Window_Red_old(const UBYTE *image, UBYTE count) {
    UBYTE k;
    if(count == 0)
        SendCommand(0x13);
    
    for (UWORD j = 0; j < 100; j++) {
        for (UWORD i = 0; i < 50; i++) {
            if(i<16)
            {
                SendData(image[i + (j*16)]);
            }
            else
            {
                SendData(0xFF);
            } 
        }
    }
}
void Epd::Display_Window_Red(const UBYTE *image, UBYTE count) {
    if(flag == 0)
        Display_Window_Red_new(image, count);
    else
        Display_Window_Red_old(image, count);
}

void Epd::DisplayFrame(void) {
    if(flag == 0)
    {
        SendCommand(0x22);
        SendData(0xF7);
        SendCommand(0x20);
        ReadBusy();
    }
    else
    {
        SendCommand(0x12);
        DelayMs(100);
        ReadBusy();
    }
}

/**
 * @brief: refresh and displays the frame
 */
void Epd::Display_old(const UBYTE *blackimage, const UBYTE *ryimage) {
    UBYTE k;
    SendCommand(0x10);
    for (UWORD j = 0; j < height; j++) {
        for (UWORD i = 0; i < width/8; i++) {
          SendData(pgm_read_byte(&blackimage[i + (j*width/8)]));
        }
    }
    
    SendCommand(0x13);
    for (UWORD j = 0; j < height; j++) {
        for (UWORD i = 0; i < width/8; i++) {
          SendData(pgm_read_byte(&ryimage[i + (j*width/8)]));
        }
    }
    SendCommand(0x12);
    DelayMs(100);
    ReadBusy();
}

void Epd::Display_new(const UBYTE *blackimage, const UBYTE *ryimage) {
    UBYTE k;
    SendCommand(0x24);
    for (UWORD j = 0; j < height; j++) {
        for (UWORD i = 0; i < width/8; i++) {
          SendData(pgm_read_byte(&blackimage[i + (j*width/8)]));
        }
    }
    
    SendCommand(0x26);
    for (UWORD j = 0; j < height; j++) {
        for (UWORD i = 0; i < width/8; i++) {
          k = pgm_read_byte(&ryimage[i + (j*width/8)]);
          SendData(~k);
        }
    }
    SendCommand(0x22);
    SendData(0xF7);
    SendCommand(0x20);
    ReadBusy();
}

void Epd::Display(const UBYTE *blackimage, const UBYTE *ryimage) {
    if(flag == 0)
        Display_new(blackimage, ryimage);
    else
        Display_old(blackimage, ryimage);
}

/**
 * @brief: clear the frame data from the SRAM, this won't refresh the display
 */
void Epd::Clear_new() {
    //send black data
    SendCommand(0x24);
    for (UWORD j = 0; j < height; j++) {
        for (UWORD i = 0; i < width/8; i++) {
            SendData(0xff);
        }
    }
    //send red data
    SendCommand(0x26);
    for (UWORD j = 0; j < height; j++) {
        for (UWORD i = 0; i < width/8; i++) {
            SendData(0x00);
        }
    }
    SendCommand(0x22);
    SendData(0xF7);
    SendCommand(0x20);
    ReadBusy();
}

void Epd::Clear_old() {
    //send black data
    SendCommand(0x10);
    for (UWORD j = 0; j < height; j++) {
        for (UWORD i = 0; i < width/8; i++) {
            SendData(0xff);
        }
    }
    //send red data
    SendCommand(0x13);
    for (UWORD j = 0; j < height; j++) {
        for (UWORD i = 0; i < width/8; i++) {
            SendData(0xff);
        }
    }
    SendCommand(0x12);
    DelayMs(100);
    ReadBusy();
}

void Epd::Clear() {
    if(flag == 0)
        Clear_new();
    else
        Clear_old();
}

/**
 * @brief: After this command is transmitted, the chip would enter the deep-sleep mode to save power. 
 *         The deep sleep mode would return to standby by hardware reset. The only one parameter is a 
 *         check code, the command would be executed if check code = 0xA5. 
 *         You can use Epd::Reset() to awaken and use Epd::Init() to initialize.
 */
void Epd::Sleep_new(void) {
    SendCommand(0x10);
    SendData(0x01);
}

void Epd::Sleep_old() {
   SendCommand(0X50);  	
   SendData(0xf7);
   SendCommand(0x02); 
   ReadBusy();
   SendCommand(0x07);  
   SendData(0XA5);
}

void Epd::Sleep(void) {
    if(flag == 0)
        Sleep_new();
    else
        Sleep_old();
}


/* END OF FILE */
