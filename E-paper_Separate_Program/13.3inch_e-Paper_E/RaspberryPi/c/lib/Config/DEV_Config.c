/*****************************************************************************
* | File      	:   DEV_Config.c
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
#include "DEV_Config.h"

/******************************************************************************
function:	Write GPIO
parameter:
Info:   
******************************************************************************/
void DEV_Digital_Write(UWORD Pin, UBYTE Value)
{ 
#if USE_BCM2835_LIB
    bcm2835_gpio_write(Pin, Value);
#elif USE_WIRINGPI_LIB
	digitalWrite(Pin, Value);
#endif
    
}

/******************************************************************************
function:	Read Write GPIO
parameter:
Info:   return  IO status
******************************************************************************/
UBYTE DEV_Digital_Read(UWORD Pin)
{
    UBYTE Read_value = 0;
#if USE_BCM2835_LIB
    Read_value = bcm2835_gpio_lev(Pin);
#elif USE_WIRINGPI_LIB
	Read_value = digitalRead(Pin);
#endif
	return Read_value;
}

/******************************************************************************
function:	Set GPIO mode
parameter:
Info:
******************************************************************************/
void DEV_GPIO_Mode(UWORD Pin, UWORD Mode)
{ 

#if USE_BCM2835_LIB
    if(Mode == 0 || Mode == BCM2835_GPIO_FSEL_INPT) {
		bcm2835_gpio_fsel(Pin, BCM2835_GPIO_FSEL_INPT);
	} else {
		bcm2835_gpio_fsel(Pin, BCM2835_GPIO_FSEL_OUTP);
	}
#elif USE_WIRINGPI_LIB
	if(Mode == 0 || Mode == INPUT) {
		pinMode(Pin, INPUT);
		pullUpDnControl(Pin, PUD_UP);
	} else {
		pinMode(Pin, OUTPUT);
		// Debug (" %d OUT \r\n",Pin);
	}
#endif
}

/**
 * SPI
**/
#if USE_BCM2835_LIB
    void DEV_SPI_WriteByte(uint8_t Value)
    {
        bcm2835_spi_transfer(Value);
    }

    void DEV_SPI_Write_nByte(const uint8_t *pData, uint32_t Len)
    {
        char rData[Len];
        bcm2835_spi_transfernb((char *)pData,rData,Len);
    }

#elif USE_WIRINGPI_LIB
    void DEV_SPI_WriteByte_M(uint8_t Value)
    {
        wiringPiSPIDataRW(0,&Value,1);
    }

    void DEV_SPI_Write_nByte_M(const uint8_t *pData, uint32_t Len)
    {
        wiringPiSPIDataRW(0, (char *)pData, Len);
    }

    void DEV_SPI_WriteByte_S(uint8_t Value)
    {
        wiringPiSPIDataRW(1,&Value,1);
    }

    void DEV_SPI_Write_nByte_S(const uint8_t *pData, uint32_t Len)
    {
        wiringPiSPIDataRW(1, (char *)pData, Len);
    }
#endif

void DEV_SPI_SendData(UBYTE Reg)
{
	UBYTE i,j=Reg;
	DEV_GPIO_Mode(EPD_SI0_PIN, 1);
	for(i = 0; i<8; i++)
    {
        DEV_Digital_Write(EPD_SCK_PIN, 0);     
        if (j & 0x80)
        {
            DEV_Digital_Write(EPD_SI0_PIN, 1);
        }
        else
        {
            DEV_Digital_Write(EPD_SI0_PIN, 0);
        }
        
        DEV_Digital_Write(EPD_SCK_PIN, 1);
        j = j << 1;
    }
	DEV_Digital_Write(EPD_SCK_PIN, 0);
}

void DEV_SPI_SendData_nByte(const UBYTE *pData, UDOUBLE Len)
{
    for (UDOUBLE i = 0; i < Len; i++)
    {
        DEV_SPI_SendData(pData[i]);
    }
}

UBYTE DEV_SPI_ReadData()
{
	UBYTE i,j=0xff;
	DEV_GPIO_Mode(EPD_SI0_PIN, 0);
	for(i = 0; i<8; i++)
	{
		DEV_Digital_Write(EPD_SCK_PIN, 0);
		j = j << 1;
		if (DEV_Digital_Read(EPD_SI0_PIN))
		{
				j = j | 0x01;
		}
		else
		{
				j= j & 0xfe;
		}
		DEV_Digital_Write(EPD_SCK_PIN, 1);
	}
	DEV_Digital_Write(EPD_SCK_PIN, 0);
	return j;
}

/******************************************************************************
function:	Initialization pin
parameter:
Info:
******************************************************************************/
static void DEV_GPIOConfig(void)
{
	DEV_GPIO_Mode(EPD_SCK_PIN, 1);
    DEV_GPIO_Mode(EPD_SI0_PIN, 1);
    DEV_GPIO_Mode(EPD_CS_M_PIN, 1);
    DEV_GPIO_Mode(EPD_CS_S_PIN, 1);
    DEV_GPIO_Mode(EPD_DC_PIN, 1);
    DEV_GPIO_Mode(EPD_RST_PIN, 1);
    DEV_GPIO_Mode(EPD_BUSY_PIN, 0);
    DEV_GPIO_Mode(EPD_PWR_PIN, 1);

    DEV_Digital_Write(EPD_SCK_PIN, 0);
    DEV_Digital_Write(EPD_SI0_PIN, 0);
    DEV_Digital_Write(EPD_CS_M_PIN, 0);
    DEV_Digital_Write(EPD_CS_S_PIN, 0);
    DEV_Digital_Write(EPD_DC_PIN, 0);
    DEV_Digital_Write(EPD_RST_PIN, 0);
    // DEV_Digital_Write(EPD_BUSY_PIN, 0);
    DEV_Digital_Write(EPD_PWR_PIN, 1);	
}

/******************************************************************************
function:	Module Initialize, the BCM2835 library and initialize the pins, SPI protocol
parameter:
Info:
******************************************************************************/
UBYTE DEV_ModuleInit(void)
{
#if USE_BCM2835_LIB
    if(!bcm2835_init()) {
		printf("bcm2835 init failed  !!! \r\n");
		return 1;
	} else {
		printf("bcm2835 init success !!! \r\n");
	}

	// GPIO Config
	// DEV_GPIOConfig();
	// bcm2835_spi_begin();                                         //Start spi interface, set spi pin for the reuse function
	// bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);     //High first transmission
	// bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                  //spi mode 0
	// bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64);  //Frequency
	// bcm2835_spi_chipSelect(BCM2835_SPI_CS0|BCM2835_SPI_CS1);                     //set CE0
	// bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0|BCM2835_SPI_CS1, LOW);     //enable cs0
    DEV_GPIOConfig();
#elif USE_WIRINGPI_LIB
	//if(wiringPiSetup() < 0)//use wiringpi Pin number table
	if(wiringPiSetupGpio() < 0) { //use BCM2835 Pin number table
		printf("set wiringPi lib failed	!!! \r\n");
		return 1;
	} else {
		printf("set wiringPi lib success !!! \r\n");
	}

	// GPIO Config
	DEV_GPIOConfig();
	// wiringPiSPISetup(0,10000000);
	// wiringPiSPISetupMode(0, 32000000, 0);
    // wiringPiSPISetup(1,10000000);
	// wiringPiSPISetupMode(1, 32000000, 0);
#endif
	
    return 0;
}

/******************************************************************************
function:	Microsecond delay
parameter:
Info:
******************************************************************************/
void DEV_Delay_us(UWORD xus)
{
    UWORD i;
    while(xus) {
        for(i = 0; i < 10; i++);
        xus--;
    }
}

/**
 * delay x ms
**/
void DEV_Delay_ms(UDOUBLE xms)
{
#if USE_BCM2835_LIB
    bcm2835_delay(xms);
#elif USE_WIRINGPI_LIB
	delay(xms);
#endif
}


/******************************************************************************
function:	Module exits, closes SPI and BCM2835 library
parameter:
Info:
******************************************************************************/
void DEV_ModuleExit(void)
{
	// DEV_Digital_Write(EPD_SCK_PIN, 0);
    // DEV_Digital_Write(EPD_SI0_PIN, 0);

    DEV_Digital_Write(EPD_CS_M_PIN, 0);
    DEV_Digital_Write(EPD_CS_S_PIN, 0);

    DEV_Digital_Write(EPD_DC_PIN, 0);

    DEV_Digital_Write(EPD_RST_PIN, 0);
    // DEV_Digital_Write(EPD_BUSY_PIN, 0);
    DEV_Digital_Write(EPD_PWR_PIN, 0);	

#if USE_BCM2835_LIB
    // bcm2835_spi_end();
	// bcm2835_close();
#endif
}
