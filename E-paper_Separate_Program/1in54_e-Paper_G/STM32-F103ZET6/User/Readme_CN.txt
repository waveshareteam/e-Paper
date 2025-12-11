/*****************************************************************************
* | File      	:   Readme_CN.txt
* | Author      :   Waveshare team
* | Function    :   Help with use
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2019-06-14
* | Info        :   在这里提供一个中文版本的使用文档，以便你的快速使用
******************************************************************************/
这个文件是帮助您使用本例程。
由于我们的墨水屏越来越多，不便于我们的维护，因此把所有的墨水屏程序做成一个工程。
在这里简略的描述本工程的使用：

1.基本信息：
本例程是基于HAL库进行开发的，对应的HAL固件版本为：STM32Cube_FW_F1_V1.6.1;
本例程是基于STM32F103ZETx进行开发的，例程均在我们的Open103Z开发板进行了验证;
本例程使用e-paper Driver HAT模块进行了验证，你可以在工程的Examples\中查看对应的测试例程;

2.管脚连接：
管脚连接你可以在Config\目录下查看DEV_Config.h中查看，这里也再重述一次：
EPD    =>    STM32f103Z
VCC    ->    3.3
GND    ->    GND
DIN    ->    PA7(MOSI)
CLK    ->    PA5(SCLK)
CS     ->    PA4
DC     ->    PA2
RST    ->    PA1
BUSY   ->    PA3

3.基本使用：
由于本工程是一个综合工程，对于使用而言，你可能需要阅读以下内容：
你可以在main.c中的73行到97行看到已经进行了注释的19个函数，
请注意你购买的是哪一款的墨水屏。
栗子1：
    如果你购买的5.83inch e-paper，那么你应该把对应的93行代码的注释去掉，即：
        //		EPD_5in83_test();
    修改成：
        EPD_5in83_test();
栗子2：
    如果你购买的2.9inch e-paper (B)，由于2.9寸的B型和C型是公用的驱动代码，
    那么你应该把对应的82行代码的注释去掉，即：
        //		EPD_2in9bc_test();
    修改成：
        EPD_2in9bc_test();
注意：对于EPD_1in54_V2_test()和EPD_2in13_V2_test()请注意你的屏幕背面是否贴有V2标识。

4.目录结构（选读）：
如果你经常使用我们的产品，对我们的程序目录结构会十分熟悉，关于具体的函数的我们有一份
函数的API手册，你可以在我们的WIKI上下载或像售后客服索取，这里简单介绍一次：
Config\:此目录为硬件接口层文件，在DEV_Config.c(.h)可以看到很多定义，包括：
    数据类型：
        #define UBYTE   uint8_t
        #define UWORD   uint16_t
        #define UDOUBLE uint32_t
    GPIO：
        #define EPD_RST_PIN     RST_GPIO_Port, RST_Pin
        #define EPD_DC_PIN      DC_GPIO_Port, DC_Pin
        #define EPD_CS_PIN      SPI_CS_GPIO_Port, SPI_CS_Pin
        #define EPD_BUSY_PIN    BUSY_GPIO_Port, BUSY_Pin
        注意：这里没有定义DIN与CLK，因为它是硬件SPI做的处理。
    读写GPIO：
        #define DEV_Digital_Write(_pin, _value) HAL_GPIO_WritePin(_pin, _value == 0? GPIO_PIN_RESET:GPIO_PIN_SET)
        #define DEV_Digital_Read(_pin) HAL_GPIO_ReadPin(_pin)
    SPI传输数据：
        void DEV_SPI_WriteByte(UBYTE value);
    延时：
        #define DEV_Delay_ms(__xms) HAL_Delay(__xms);
        注意：此延时函数并未使用示波器测量具体数值
    模块初始化与退出的处理：
        void DEV_Module_Init(void);
        void DEV_Module_Exit(void);
        注意：1.这里是处理使用墨水屏前与使用完之后一些GPIO的处理。
              2.对于PCB带有Rev2.1的，DEV_Module_Exit()之后整个模块会进入低功耗，经过测试这个功耗基本为0;
              
GUI\:此目录为一些基本的图像处理函数，在GUI_Paint.c(.h)中：
    常用图像处理：创建图形、翻转图形、镜像图形、设置像素点、清屏等;
    常用画图处理：画点、线、框、圆、中文字符、英文字符、数字等;
    常用时间显示：提供一个常用的显示时间函数;
    常用显示图片：提供一个显示位图的函数;
    
Fonts\:为一些常用的字体：
    Ascii:
        font8: 5*8 
        font12: 7*12
        font16: 11*16 
        font20: 14*20 
        font24: 17*24
    中文：
        font12CN: 16*21 
        font24CN: 32*41
        
e-paper\:此目录下为墨水屏驱动函数;
Examples\:此目录下为墨水屏的测试程序，你可在其中看到具体的使用方法;