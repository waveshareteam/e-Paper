/**
 *  @filename   :   epd5in79.h
 *  @brief      :   Header file for e-paper library epd5in83.cpp
 *  @author     :   MyMX from Waveshare
 *  
 *  Copyright (C) Waveshare     2024/03/06
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
#include "epd5in79.h"

Epd::~Epd() {
};

Epd::Epd() {
    reset_pin = RST_PIN;
    dc_pin = DC_PIN;
    cs_pin = CS_PIN;
    busy_pin = BUSY_PIN;
    width = EPD_WIDTH;
    height = EPD_HEIGHT;
};

int Epd::Init(void) {
    if (IfInit() != 0) {
        return -1;
    }
    Reset();

    ReadBusy();        //waiting for the electronic paper IC to release the idle signal
    SendCommand(0x12); //POWER ON
    ReadBusy();        //waiting for the electronic paper IC to release the idle signal

    SendCommand(0x11);
    SendData(0x01);

    SendCommand(0x44);	 						 // Set Ram X- address Start / End position
    SendData(0x00);     						 // XStart, POR = 00h
    SendData(0x31); //400/8-1
    SendCommand(0x45);	 									// Set Ram Y- address  Start / End position 
    SendData(0x0f);  
    SendData(0x01);  //300-1	
    SendData(0x00);     									// YEnd L
    SendData(0x00);											// YEnd H 

    SendCommand(0x4e);	 						 
    SendData(0x00);	
    SendCommand(0x4f);	 
    SendData(0x0f);  
    SendData(0x01); 	

    ReadBusy();   

    SendCommand(0x91);
    SendData(0x00);

    SendCommand(0xC4);							 // Set Ram X- address Start / End position
    SendData(0x31);     						 // XStart, POR = 00h
    SendData(0x00); //400/8-1
    SendCommand(0xC5);	 									// Set Ram Y- address  Start / End position 
    SendData(0x0f);  
    SendData(0x01);  //300-1	
    SendData(0x00);     									// YEnd L
    SendData(0x00);											// YEnd H 

    SendCommand(0xCE);	 						 
    SendData(0x31);	
    SendCommand(0xCF);	 
    SendData(0x0f);  
    SendData(0x01);

    ReadBusy();   

    return 0;
}

int Epd::Init_Fast(void) {
    if (IfInit() != 0) {
        return -1;
    }
    Reset();

    ReadBusy();        //waiting for the electronic paper IC to release the idle signal
	SendCommand(0x12); //POWER ON
	ReadBusy();        //waiting for the electronic paper IC to release the idle signal
    
    SendCommand(0x18); //Read built-in temperature sensor
    SendData(0x80);	

    SendCommand(0x22); // Load temperature value
    SendData(0xB1);		
    SendCommand(0x20);	
    ReadBusy();   

    SendCommand(0x1A); // Write to temperature register
    SendData(0x64);		
    SendData(0x00);	
                
    SendCommand(0x22); // Load temperature value
    SendData(0x91);		
    SendCommand(0x20);	
    ReadBusy();   

    SendCommand(0x11);
    SendData(0x01);
    
    SendCommand(0x44);	 						 // Set Ram X- address Start / End position
	SendData(0x00);     						 // XStart, POR = 00h
	SendData(0x31); //400/8-1
	SendCommand(0x45);	 									// Set Ram Y- address  Start / End position 
	SendData(0x0f);  
	SendData(0x01);  //300-1	
	SendData(0x00);     									// YEnd L
	SendData(0x00);											// YEnd H 

    SendCommand(0x4e);	 						 
	SendData(0x00);	
	SendCommand(0x4f);	 
	SendData(0x0f);  
	SendData(0x01); 	

    ReadBusy();   
    
    SendCommand(0x91);
    SendData(0x00);

    SendCommand(0xC4);							 // Set Ram X- address Start / End position
	SendData(0x31);     						 // XStart, POR = 00h
	SendData(0x00); //400/8-1
	SendCommand(0xC5);	 									// Set Ram Y- address  Start / End position 
	SendData(0x0f);  
	SendData(0x01);  //300-1	
	SendData(0x00);     									// YEnd L
	SendData(0x00);											// YEnd H 

    SendCommand(0xCe);	 						 
	SendData(0x31);	
	SendCommand(0xCf);	 
	SendData(0x0f);  
	SendData(0x01);

    ReadBusy();   

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
    while(DigitalRead(busy_pin) == 1) {      //0: busy, 1: idle
        DelayMs(100);
    }      
}

/**
 *  @brief: module reset.
 *          often used to awaken the module in deep sleep,
 *          see Epd::Sleep();
 */
void Epd::Reset(void) {
    DigitalWrite(reset_pin, LOW);                //module reset    
    DelayMs(200);
    DigitalWrite(reset_pin, HIGH);
    DelayMs(200);    
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
void Epd::TurnOnDisplay(void)
{
	SendCommand(0x22); //Display Update Control
	SendData(0xF7);
	SendCommand(0x20); //Activate Display Update Sequence
	ReadBusy();
}

void Epd::TurnOnDisplay_Fast(void)
{
	SendCommand(0x22); //Display Update Control
	SendData(0xC7);
	SendCommand(0x20); //Activate Display Update Sequence
	ReadBusy();
}

void Epd::TurnOnDisplay_Part(void)
{
	SendCommand(0x22); //Display Update Control
	SendData(0xFF);
	SendCommand(0x20); //Activate Display Update Sequence
	ReadBusy();
}

void Epd::TurnOnDisplay_4GRAY(void)
{
    SendCommand(0x22);
    SendData(0xCF);
	SendCommand(0x20);
    ReadBusy();
}

void Epd::Display(const unsigned char* frame_buffer) {
    int Width, Width1, Height;
    Width =(width % 16 == 0)?(width / 16 ):(width / 16 + 1);
    Width1 =(width % 8 == 0)?(width / 8 ):(width / 8 + 1);
    Height = height;

    // M part 396*272
    SendCommand(0x24);
    for (int j = 0; j < Height; j++) {
        for (int i = 0; i < Width; i++) {
            SendData(pgm_read_byte(&(frame_buffer[j * Width1 + i])));
        }
    }

    SendCommand(0X26);
    for (int j = 0; j < Height; j++) {
        for (int i = 0; i < Width; i++) {
            SendData(0x00);
        }
    }

    // S part 396*272
    SendCommand(0xA4);
    for (int j = 0; j < Height; j++) {
        for (int i = 0; i < Width; i++) {
            SendData(pgm_read_byte(&(frame_buffer[j * Width1 + i + Width - 1])));
        }
    }

    SendCommand(0XA6);
    for (int j = 0; j < Height; j++) {
        for (int i = 0; i < Width; i++) {
            SendData(0x00);
        }
    }

    TurnOnDisplay();
}

void Epd::Display_Fast(const unsigned char* frame_buffer) {
    int Width, Width1, Height;
    Width =(width % 16 == 0)?(width / 16 ):(width / 16 + 1);
    Width1 =(width % 8 == 0)?(width / 8 ):(width / 8 + 1);
    Height = height;

    // M part 396*272
    SendCommand(0x24);
    for (int j = 0; j < Height; j++) {
        for (int i = 0; i < Width; i++) {
            SendData(frame_buffer[j * Width1 + i]);
        }
    }

    SendCommand(0X26);
    for (int j = 0; j < Height; j++) {
        for (int i = 0; i < Width; i++) {
            SendData(0x00);
        }
    }

    // S part 396*272
    SendCommand(0xA4);
    for (int j = 0; j < Height; j++) {
        for (int i = 0; i < Width; i++) {
            SendData(frame_buffer[j * Width1 + i + Width - 1]);
        }
    }

    SendCommand(0XA6);
    for (int j = 0; j < Height; j++) {
        for (int i = 0; i < Width; i++) {
            SendData(0x00);
        }
    }

    TurnOnDisplay_Fast();
}

void Epd::Clear(void) {
    
    SendCommand(0x24);
    // M part 396*272
    SendCommand(0x24);
    for(int i=0; i<13600; i++) 
    {
        SendData(0xFF);
    }

    SendCommand(0X26);
    for(int i=0; i<13600; i++) 
    {
        SendData(0x00);
    }

    // S part 396*272
    SendCommand(0xA4);
    for(int i=0; i<13600; i++) 
    {
        SendData(0xFF);
    }

    SendCommand(0XA6);
    for(int i=0; i<13600; i++) 
    {
        SendData(0x00);
    }

    TurnOnDisplay();
}


void Epd::Sleep(void) {
    SendCommand(0X10);
    SendData(0x01);
}

/* END OF FILE */


