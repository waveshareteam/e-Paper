/**
 *  @filename   :   epdif.cpp
 *  @brief      :   Implements EPD interface functions
 *                  Users have to implement all the functions in epdif.cpp
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

#include "epdif.h"
#include <SPI.h>

EpdIf::EpdIf() {
};

EpdIf::~EpdIf() {
};

void EpdIf::DigitalWrite(int pin, int value) {
    digitalWrite(pin, value);
}

int EpdIf::DigitalRead(int pin) {
    return digitalRead(pin);
}

void EpdIf::DelayMs(unsigned int delaytime) {
    delay(delaytime);
}

void EpdIf::SpiTransfer(unsigned char data) {
    digitalWrite(CS_PIN, LOW);
    SPI.transfer(data);
    digitalWrite(CS_PIN, HIGH);
}

void EpdIf::EPD_GPIO_Init()
{
    SPI.end(); 
}

void EpdIf::EPD_SPI_Init()
{
    SPI.begin();
    SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
}

void EpdIf::EPD_Mode(int pin, char mode) {
    if(mode == 0)
        pinMode(pin, INPUT);
    else
        pinMode(pin, OUTPUT);
}

void EpdIf::EPD_SendData(unsigned char data) {
    unsigned char i,j=data;
	EPD_Mode(MOSI_PIN, 1);
    EPD_Mode(SCLK_PIN, 1);
	digitalWrite(CS_PIN, 0);
	for(i = 0; i<8; i++)
    {
        digitalWrite(SCLK_PIN, 0);     
        if (j & 0x80)
        {
            digitalWrite(MOSI_PIN, 1);
        }
        else
        {
            digitalWrite(MOSI_PIN, 0);
        }
        
        digitalWrite(SCLK_PIN, 1);
        j = j << 1;
        // DelayMs(1);
    }
	digitalWrite(SCLK_PIN, 0);
	digitalWrite(CS_PIN, 1);
}

unsigned char  EpdIf::EPD_ReadData()
{
    unsigned char i,j=0xff;
    EPD_Mode(MOSI_PIN, 0);
    EPD_Mode(SCLK_PIN, 1);
    digitalWrite(CS_PIN, 0);
    for(i = 0; i<8; i++)
    {
        digitalWrite(SCLK_PIN, 0);
        j = j << 1;
        if (DigitalRead(MOSI_PIN))
        {
                j = j | 0x01;
        }
        else
        {
                j= j & 0xfe;
        }
        digitalWrite(SCLK_PIN, 1);
    }
    digitalWrite(SCLK_PIN, 0);
    digitalWrite(CS_PIN, 1);
    return j;
}


int EpdIf::IfInit(void) {
    pinMode(CS_PIN, OUTPUT);
    pinMode(RST_PIN, OUTPUT);
    pinMode(DC_PIN, OUTPUT);
    pinMode(BUSY_PIN, INPUT); 

    pinMode(PWR_PIN, OUTPUT);
    DigitalWrite(PWR_PIN, 1);

    SPI.begin();
    SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
    return 0;
}

