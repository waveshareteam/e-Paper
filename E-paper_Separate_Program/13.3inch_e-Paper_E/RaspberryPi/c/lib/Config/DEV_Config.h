/*****************************************************************************
* | File      	:   DEV_Config.h
* | Author      :   Waveshare team
* | Function    :   Hardware underlying interface
* | Info        :
*                Used to shield the underlying layers of each master
*                and enhance portability,software spi.
*----------------
* |	This version:   V1.0
* | Date        :   2018-11-29
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
#ifndef _DEV_CONFIG_H_
#define _DEV_CONFIG_H_


#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#ifdef USE_BCM2835_LIB
    #include <bcm2835.h>
#elif USE_WIRINGPI_LIB
    #include <wiringPi.h>
    #include <wiringPiSPI.h>
#endif

/**
 * data
**/
#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t

/**
 * GPIO config
**/
#define EPD_SCK_PIN     11
#define EPD_SI0_PIN     10

#define EPD_CS_M_PIN    8
#define EPD_CS_S_PIN    7

#define EPD_DC_PIN      25

#define EPD_RST_PIN     17
#define EPD_BUSY_PIN    24
#define EPD_PWR_PIN     18

// Used to decide whether to use hardware SPI or analog SPI
#define Hardware_SPI 0



/*------------------------------------------------------------------------------------------------------*/
void DEV_Digital_Write(UWORD Pin, UBYTE Value);
UBYTE DEV_Digital_Read(UWORD Pin);

void DEV_Delay_us(UWORD xus);
void DEV_Delay_ms(UDOUBLE xms);

#if USE_BCM2835_LIB
void DEV_SPI_WriteByte(uint8_t Value);
void DEV_SPI_Write_nByte(const uint8_t *pData, uint32_t Len);
#else
void DEV_SPI_WriteByte_M(uint8_t Value);
void DEV_SPI_Write_nByte_M(const uint8_t *pData, uint32_t Len);
void DEV_SPI_WriteByte_S(uint8_t Value);
void DEV_SPI_Write_nByte_S(const uint8_t *pData, uint32_t Len);
#endif

void DEV_SPI_SendData(UBYTE Reg);
void DEV_SPI_SendData_nByte(const UBYTE *pData, UDOUBLE Len);
UBYTE DEV_SPI_ReadData();


UBYTE DEV_ModuleInit(void);
void DEV_ModuleExit(void);


#endif
