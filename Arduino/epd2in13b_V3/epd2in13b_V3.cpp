/**
 *  @filename   :   epd2in9b.cpp
 *  @brief      :   Implements for Dual-color e-paper library
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     August 10 2017
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
#include "epd2in13b_V3.h"

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
    
    SendCommand(0x04);  
    WaitUntilIdle();//waiting for the electronic paper IC to release the idle signal

    SendCommand(0x00);//panel setting
    SendData(0x0f);//LUT from OTP，128x296
    SendData(0x89);//Temperature sensor, boost and other related timing settings

    SendCommand(0x61);//resolution setting
    SendData (0x68);
    SendData (0x00);
    SendData (0xD4);

    SendCommand(0X50);//VCOM AND DATA INTERVAL SETTING
    SendData(0x77);//WBmode:VBDF 17|D7 VBDW 97 VBDB 57
                                 //WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7;
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
    SendCommand(0x71);
    while(DigitalRead(busy_pin) == 0) {      //0: busy, 1: idle
        SendCommand(0x71);
        DelayMs(50);
    }
    DelayMs(200);
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
    DelayMs(10);
    DigitalWrite(reset_pin, HIGH);
    DelayMs(200); 
    this->count = 0;    
}


/**
 * @brief: refresh and displays the frame
 */
void Epd::DisplayFrame(const unsigned char* frame_buffer_black, const unsigned char* frame_buffer_red) {
    if (frame_buffer_black != NULL) {
        SendCommand(0x10);
        for (int i = 0; i < this->width * this->height / 8; i++) {
            SendData(pgm_read_byte(&frame_buffer_black[i]));
        }
        DelayMs(2);
    }
    if (frame_buffer_red != NULL) {
        SendCommand(0x13);
        for (int i = 0; i < this->width * this->height / 8; i++) {
            SendData(pgm_read_byte(&frame_buffer_red[i]));
        }
        
    }
    SendCommand(0x12);
    DelayMs(100);
    WaitUntilIdle();
    this->count = 0;
}

void Epd::Display(const unsigned char* frame_buffer) {
    // 1378
    if(this->count == 0){
        SendCommand(0x10);
        this->count++;
    }else if(this->count > 0 && this->count < 4 ){
        this->count++;
    }else if(this->count == 4){
        SendCommand(0x13);
        this->count++;
    }else if(this->count > 4 && this->count < 8 ){
        this->count++;
    }
    for(int i = 0; i < this->width * this->height / 8 / 4; i++){
            SendData(frame_buffer[i]);
    }
    if(this->count == 8){
        SendCommand(0x12);
        DelayMs(100);
        WaitUntilIdle();
        this->count = 0;
    }
}

/**
 * @brief: clear the frame data from the SRAM, this won't refresh the display
 */
void Epd::ClearFrame(void) {
    SendCommand(0x10);           
    for(int i = 0; i < width * height / 8; i++) {
        SendData(0xFF);  
    }  
    DelayMs(2);
    SendCommand(0x13);
    for(int i = 0; i < width * height / 8; i++) {
        SendData(0xFF);  
    }
    SendCommand(0x12);
    DelayMs(100);
    WaitUntilIdle();
    this->count = 0;
}

/**
 * @brief: This displays the frame data from SRAM
 */
void Epd::DisplayFrame(void) {
    this->count = 0;
    SendCommand(0x12);
    DelayMs(100);    
    WaitUntilIdle();
    
}

/**
 * @brief: After this command is transmitted, the chip would enter the deep-sleep mode to save power. 
 *         The deep sleep mode would return to standby by hardware reset. The only one parameter is a 
 *         check code, the command would be executed if check code = 0xA5. 
 *         You can use Epd::Reset() to awaken and use Epd::Init() to initialize.
 */
void Epd::Sleep() {
    SendCommand(0X50);
    SendData(0xf7);
    SendCommand(0X02);//power off
    WaitUntilIdle();//waiting for the electronic paper IC to release the idle signal
    SendCommand(0X07);//deep sleep
    SendData(0xA5);
    this->count = 0;
}


/* END OF FILE */
