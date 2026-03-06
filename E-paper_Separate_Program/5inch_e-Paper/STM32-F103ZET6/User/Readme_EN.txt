/*****************************************************************************
* | File      	:   Readme_CN.txt
* | Author      :   Waveshare team
* | Function    :   Help with use
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2019-06-14
* | Info        :   Here is an English version of the documentation for your quick use.
******************************************************************************/
This file is to help you use this routine.
Since our ink screens are getting more and more, it is not convenient for our maintenance, so all the ink screen programs are made into one project.
A brief description of the use of this project is here:

1. Basic information:
This routine is developed based on the HAL library, and the corresponding HAL firmware version is: STM32Cube_FW_F1_V1.6.1;
This routine was developed based on STM32F103ZETx, and the routines were verified on our Open103Z development board;
This routine has been verified using the e-paper Driver HAT module. You can view the corresponding test routines in the
Examples\ of the project;

2. Pin connection:
Pin connection You can view it in DEV_Config.h in the Config\ directory, and repeat it here:
EPD    =>    STM32f103Z
VCC    ->    3.3
GND    ->    GND
DIN    ->    PA7(MOSI)
CLK    ->    PA5(SCLK)
CS     ->    PA4
DC     ->    PA2
RST    ->    PA1
BUSY   ->    PA3

3. Basic use:
Since this project is a comprehensive project, you may need to read the following for use:
You can see the 19 functions that have been annotated in lines 73 through 97 of main.c.
Please note which ink screen you purchased.
example 1:
	If you purchased 5.83inch e-paper, then you should remove the comment for the corresponding 93 lines of code, ie:
        //		EPD_5in83_test();
	changed to:
		EPD_5in83_test();
example 2:
    If you buy a 2.9inch e-paper (B), since the 2.9-inch Type B and Type C are common driver codes,
    Then you should remove the comment for the corresponding 82 lines of code, ie:
        // EPD_2in9bc_test();
    changed to:
        EPD_2in9bc_test();
Note: For EPD_1in54_V2_test() and EPD_2in13_V2_test(), please note that the V2 logo is attached to the back of your screen.

4. Directory structure (selection):
If you use our products frequently, we will be very familiar with our program directory structure. We have a copy of the specific function.
The API manual for the function, you can download it on our WIKI or request it as an after-sales customer service. Here is a brief introduction:
Config\: This directory is a hardware interface layer file. You can see many definitions in DEV_Config.c(.h), including:
    type of data:
        #define UBYTE uint8_t
        #define UWORD uint16_t
        #define UDOUBLE uint32_t
    GPIO:
        #define EPD_RST_PIN RST_GPIO_Port, RST_Pin
        #define EPD_DC_PIN DC_GPIO_Port, DC_Pin
        #define EPD_CS_PIN SPI_CS_GPIO_Port, SPI_CS_Pin
        #define EPD_BUSY_PIN BUSY_GPIO_Port, BUSY_Pin
        Note: DIN and CLK are not defined here because it is handled by the hardware SPI.
    Read and write GPIO:
        #define DEV_Digital_Write(_pin, _value) HAL_GPIO_WritePin(_pin, _value == 0? GPIO_PIN_RESET: GPIO_PIN_SET)
        #define DEV_Digital_Read(_pin) HAL_GPIO_ReadPin(_pin)
    SPI transfer data:
        Void DEV_SPI_WriteByte(UBYTE value);
    Delay:
        #define DEV_Delay_ms(__xms) HAL_Delay(__xms);
        Note: This delay function does not use an oscilloscope to measure specific values.
    Module Init and exit processing:
        Void DEV_Module_Init(void);
        Void DEV_Module_Exit(void);
        Note: 1. Here is the processing of some GPIOs before and after using the ink screen.
              2. For the PCB with Rev2.1, the entire module will enter low power consumption after DEV_Module_Exit(). After testing, the power consumption is basically 0;
            
GUI\: This directory is some basic image processing functions, in GUI_Paint.c(.h):
    Common image processing: creating graphics, flipping graphics, mirroring graphics, setting pixels, clearing screens, etc.
    Common drawing processing: drawing points, lines, boxes, circles, Chinese characters, English characters, numbers, etc.;
    Common time display: Provide a common display time function;
    Commonly used display pictures: provide a function to display bitmaps;
    
	Fonts\: for some commonly used fonts:
    Ascii:
        Font8: 5*8
        Font12: 7*12
        Font16: 11*16
        Font20: 14*20
        Font24: 17*24
    Chinese:
        font12CN: 16*21
        font24CN: 32*41
        
E-paper\: This screen is the ink screen driver function;
Examples\: This is the test program for the ink screen. You can see the specific usage method in it.