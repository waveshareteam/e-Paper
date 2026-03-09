/*****************************************************************************
* | File        :   dev_hardware_SPI.h
* | Author      :   Waveshare team
* | Function    :   Read and write /dev/SPI,  hardware SPI
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2019-06-26
* | Info        :   Basic version
*
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
#ifndef __DEV_HARDWARE_SPI_
#define __DEV_HARDWARE_SPI_

#include <stdint.h>

#define DEV_HARDWARE_SPI_DEBUG 0
#if DEV_HARDWARE_SPI_DEBUG
#define DEV_HARDWARE_SPI_Debug(__info,...) printf("Debug: " __info,##__VA_ARGS__)
#else
#define DEV_HARDWARE_SPI_Debug(__info,...)
#endif

#define SPI_CPHA        0x01
#define SPI_CPOL        0x02
#define SPI_MODE_0      (0|0)
#define SPI_MODE_1      (0|SPI_CPHA)
#define SPI_MODE_2      (SPI_CPOL|0)
#define SPI_MODE_3      (SPI_CPOL|SPI_CPHA)

typedef enum{
    SPI_MODE0 = SPI_MODE_0,  /*!< CPOL = 0, CPHA = 0 */
    SPI_MODE1 = SPI_MODE_1,  /*!< CPOL = 0, CPHA = 1 */
    SPI_MODE2 = SPI_MODE_2,  /*!< CPOL = 1, CPHA = 0 */
    SPI_MODE3 = SPI_MODE_3   /*!< CPOL = 1, CPHA = 1 */
}SPIMode;

typedef enum{
    DISABLE  = 0,
    ENABLE   = 1
}SPICSEN;

typedef enum{
    SPI_CS_Mode_LOW  = 0,     /*!< Chip Select 0 */
    SPI_CS_Mode_HIGH = 1,     /*!< Chip Select 1 */
    SPI_CS_Mode_NONE = 3  /*!< No CS, control it yourself */
}SPIChipSelect;

typedef enum
{
    SPI_BIT_ORDER_LSBFIRST = 0,  /*!< LSB First */
    SPI_BIT_ORDER_MSBFIRST = 1   /*!< MSB First */
}SPIBitOrder;

typedef enum
{
    SPI_3WIRE_Mode = 0,
    SPI_4WIRE_Mode = 1
}BusMode;


/**
 * Define SPI attribute
**/
typedef struct SPIStruct {
    //GPIO
    uint16_t SCLK_PIN;
    uint16_t MOSI_PIN;
    uint16_t MISO_PIN;
    
    uint16_t CS0_PIN;
    uint16_t CS1_PIN;
    
    
    uint32_t speed;
    uint16_t mode;
    uint16_t delay;
    int fd; //
} HARDWARE_SPI;




void DEV_HARDWARE_SPI_begin(char *SPI_device);
void DEV_HARDWARE_SPI_beginSet(char *SPI_device, SPIMode mode, uint32_t speed);
void DEV_HARDWARE_SPI_end(void);

int DEV_HARDWARE_SPI_setSpeed(uint32_t speed);

uint8_t DEV_HARDWARE_SPI_TransferByte(uint8_t buf);
int DEV_HARDWARE_SPI_Transfer(uint8_t *buf, uint32_t len);

void DEV_HARDWARE_SPI_SetDataInterval(uint16_t us);
int DEV_HARDWARE_SPI_SetBusMode(BusMode mode);
int DEV_HARDWARE_SPI_SetBitOrder(SPIBitOrder Order);
int DEV_HARDWARE_SPI_ChipSelect(SPIChipSelect CS_Mode);
int DEV_HARDWARE_SPI_CSEN(SPICSEN EN);
int DEV_HARDWARE_SPI_Mode(SPIMode mode);


#endif