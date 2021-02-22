/**
 *  @filename   :   epd2in7b_V2.cpp
 *  @brief      :   Implements for Dual-color e-paper library
 *  @author     :   
 *
 *  Copyright (C) Waveshare     Feb 19 2021
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
#include "epd2in7b_V2.h"

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
    /* this calls the peripheral hardware interface, see epdif */
    if (IfInit() != 0) {
        return -1;
    }
    /* EPD hardware init start */
    Reset();
     
    WaitUntilIdle();
	SendCommand(0x12);
	WaitUntilIdle();
	
    SendCommand(0x00);
    SendData(0x27);
    SendData(0x01);
	SendData(0x00);
	
    SendCommand(0x11);
    SendData(0x03); 
    
	SetWindows(0, 0, width-1, height-1);
	SetCursor(0, 0);
    /* EPD hardware init end */
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
    while(DigitalRead(busy_pin) == 1) {      //0: busy, 1: idle
        DelayMs(10);
    }      
}

/******************************************************************************
function :	Setting the display window
parameter:
******************************************************************************/
void Epd::SetWindows(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend)
{
    SendCommand(0x44); // SET_RAM_X_ADDRESS_START_END_POSITION
    SendData((Xstart>>3) & 0xFF);
    SendData((Xend>>3) & 0xFF);
	
    SendCommand(0x45); // SET_RAM_Y_ADDRESS_START_END_POSITION
    SendData(Ystart & 0xFF);
    SendData((Ystart >> 8) & 0xFF);
    SendData(Yend & 0xFF);
    SendData((Yend >> 8) & 0xFF);
}

/******************************************************************************
function :	Set Cursor
parameter:
******************************************************************************/
void Epd::SetCursor(UWORD Xstart, UWORD Ystart)
{
    SendCommand(0x4E); // SET_RAM_X_ADDRESS_COUNTER
    SendData(Xstart & 0xFF);

    SendCommand(0x4F); // SET_RAM_Y_ADDRESS_COUNTER
    SendData(Ystart & 0xFF);
    SendData((Ystart >> 8) & 0xFF);
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
void Epd::TransmitPartialBlack(const unsigned char* buffer_black, int x, int y, int w, int l) {
    if (buffer_black != NULL) {
		
		int width = (w%8 == 0 ? w/8 : w/8+1);
		
		SetWindows(x, y, w+x-1, l+y-1);
		SetCursor(x, y);
        SendCommand(0x24);
        DelayMs(2);
        for(int i = 0; i < width * l; i++) {
            SendData(~buffer_black[i]);  
        }  
        DelayMs(2);
    }
}

/**
 *  @brief: transmit partial data to the red part of SRAM
 */
void Epd::TransmitPartialRed(const unsigned char* buffer_red, int x, int y, int w, int l) {
    if (buffer_red != NULL) {
		
		int width = (w%8 == 0 ? w/8 : w/8+1);
		
		SetWindows(x, y, w+x-1, l+y-1);
		SetCursor(x, y);
        SendCommand(0x26);
        DelayMs(2);
        for(int i = 0; i < width * l; i++) {
            SendData(buffer_red[i]);  
        }  
        DelayMs(2);
    }
}

/**
 * @brief: refresh and displays the frame
 */
void Epd::DisplayFrame(const unsigned char* frame_buffer_black, const unsigned char* frame_buffer_red) {
    if (frame_buffer_black != NULL) {
        SendCommand(0x24);           
        DelayMs(2);
        for(int i = 0; i < width * height / 8; i++) {
            SendData(~pgm_read_byte(&frame_buffer_black[i]));  
        }  
        DelayMs(2);
    }
    if (frame_buffer_red != NULL) {
        SendCommand(0x26);
        DelayMs(2);
        for(int i = 0; i < width * height / 8; i++) {
            SendData(pgm_read_byte(&frame_buffer_red[i]));  
        }  
        DelayMs(2);
    }
	DisplayFrame();
}

/**
 * @brief: clear the frame data from the SRAM, this won't refresh the display
 */
void Epd::ClearFrame(void) {
    SendCommand(0x24);           
    DelayMs(2);
    for(int i = 0; i < width * height / 8; i++) {
        SendData(0xff);  
    }  
    DelayMs(2);
    SendCommand(0x26);
    DelayMs(2);
    for(int i = 0; i < width * height / 8; i++) {
        SendData(0x00);  
    }  
    DelayMs(2);
	DisplayFrame();
}

/**
 * @brief: This displays the frame data from SRAM
 */
void Epd::DisplayFrame(void) {
    SendCommand(0x20); 
    WaitUntilIdle();
}

/**
 * @brief: After this command is transmitted, the chip would enter the deep-sleep mode to save power. 
 *         The deep sleep mode would return to standby by hardware reset. The only one parameter is a 
 *         check code, the command would be executed if check code = 0xA5. 
 *         You can use Epd::Reset() to awaken and use Epd::Init() to initialize.
 */
void Epd::Sleep() {
  SendCommand(0x10);
  SendData(0x01);
}

/* END OF FILE */
