/**
 *  @filename   :   epd2in13b_V4.cpp
 *  @brief      :   Implements for Three-color e-paper library
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     April 25 2022
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
#include "epd2in13b_V4.h"

Epd::~Epd() {
};

Epd::Epd() {
    reset_pin = RST_PIN;
    dc_pin = DC_PIN;
    cs_pin = CS_PIN;
    busy_pin = BUSY_PIN;
    width = EPD_WIDTH;
    bufwidth = 128/8;
    height = EPD_HEIGHT;
    bufheight = 63;
};

int Epd::Init(void) {
    /* this calls the peripheral hardware interface, see epdif */
    if (IfInit() != 0) {
        return -1;
    }
    /* EPD hardware init start */
    Reset();
    
    WaitUntilIdle();
    SendCommand(0x12);  // SWRESET
    WaitUntilIdle();

	SendCommand(0x01); //Driver output control      
	SendData(0xf9);
	SendData(0x00);
	SendData(0x00);

	SendCommand(0x11); //data entry mode       
	SendData(0x03);

	SetWindows(0, 0, width-1, height-1);
	SetCursor(0, 0);

	SendCommand(0x3C); //BorderWavefrom
	SendData(0x05);	

	SendCommand(0x18); //Read built-in temperature sensor
	SendData(0x80);	

	SendCommand(0x21); //  Display update control
	SendData(0x80);	
	SendData(0x80);

	WaitUntilIdle();
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
    while(DigitalRead(busy_pin) == 1) {      //1: busy, 0: idle
        DelayMs(10);
    }
    DelayMs(10);
}

/**
 *  @brief: module reset. 
 *          often used to awaken the module in deep sleep, 
 *          see Epd::Sleep();
 */
void Epd::Reset(void) {
    DigitalWrite(reset_pin, HIGH);
    DelayMs(20);  
    DigitalWrite(reset_pin, LOW);
    DelayMs(2);
    DigitalWrite(reset_pin, HIGH);
    DelayMs(20); 
    this->count = 0;    
}


// Setting the display window
void Epd::SetWindows(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend) {
    SendCommand(0x44); // SET_RAM_X_ADDRESS_START_END_POSITION
    SendData((Xstart>>3) & 0xFF);
    SendData((Xend>>3) & 0xFF);
	
    SendCommand(0x45); // SET_RAM_Y_ADDRESS_START_END_POSITION
    SendData(Ystart & 0xFF);
    SendData((Ystart >> 8) & 0xFF);
    SendData(Yend & 0xFF);
    SendData((Yend >> 8) & 0xFF);
}

// Set Cursor
void Epd::SetCursor(UWORD Xstart, UWORD Ystart) {
    SendCommand(0x4E); // SET_RAM_X_ADDRESS_COUNTER
    SendData(Xstart & 0xFF);

    SendCommand(0x4F); // SET_RAM_Y_ADDRESS_COUNTER
    SendData(Ystart & 0xFF);
    SendData((Ystart >> 8) & 0xFF);
}

/**
 * @brief: refresh and displays the frame
 */
void Epd::DisplayFrame(const unsigned char* frame_buffer_black, const unsigned char* frame_buffer_red) {
    if (frame_buffer_black != NULL) {
        SendCommand(0x24);
        for (int i = 0; i < this->bufwidth * this->height; i++) {
            SendData(pgm_read_byte(&frame_buffer_black[i]));
        }
        DelayMs(2);
    }
    if (frame_buffer_red != NULL) {
        SendCommand(0x26);
        for (int i = 0; i < this->bufwidth * this->height; i++) {
            SendData(pgm_read_byte(&frame_buffer_red[i]));
        }
        
    }
    SendCommand(0x20);
    WaitUntilIdle();
    this->count = 0;
}

void Epd::Display(const unsigned char* frame_buffer) {
    if(this->count == 0){
        SendCommand(0x24);
        this->count++;
    }else if(this->count > 0 && this->count < 4 ){
        this->count++;
    }else if(this->count == 4){
        SendCommand(0x26);
        this->count++;
    }else if(this->count > 4 && this->count < 8 ){
        this->count++;
    }
    for(int i = 0; i < this->bufwidth * this->bufheight; i++){
            SendData(frame_buffer[i]);
    }
    if(this->count == 8){
        SendCommand(0x20);
        WaitUntilIdle();
        this->count = 0;
    }
}

/**
 * @brief: clear the frame data from the SRAM, this won't refresh the display
 */
void Epd::ClearFrame(void) {
    SendCommand(0x24);           
    for(int i = 0; i < bufwidth * height; i++) {
        SendData(0xFF);  
    }  
    SendCommand(0x26);
    for(int i = 0; i < bufwidth * height; i++) {
        SendData(0xFF);  
    }

    SendCommand(0x20);
    WaitUntilIdle();
    this->count = 0;
}

/**
 * @brief: This displays the frame data from SRAM
 */
void Epd::DisplayFrame(void) {
    this->count = 0;
    SendCommand(0x20);  // Activate Display Update Sequence
    WaitUntilIdle();
}

/**
 * @brief: After this command is transmitted, the chip would enter the deep-sleep mode to save power. 
 *         The deep sleep mode would return to standby by hardware reset. The only one parameter is a 
 *         check code, the command would be executed if check code = 0xA5. 
 *         You can use Epd::Reset() to awaken and use Epd::Init() to initialize.
 */
void Epd::Sleep() {
    SendCommand(0x10);  //enter deep sleep
    SendData(0x01);
    DelayMs(100);   
    this->count = 0;
}


/* END OF FILE */
