/*****************************************************************************
* | File      	:   Readme_CN.txt
* | Author      :   Waveshare team
* | Function    :   Help with use
* | Info        :
*----------------
* | This version:   V1.3
* | Date        :   2022-10-13
* | Info        :   在这里提供一个中文版本的使用文档，以便你的快速使用
******************************************************************************/
这个文件是帮助您使用本例程。
由于我们的墨水屏越来越多，不便于我们的维护，因此把所有的墨水屏程序做成一个工程。
在这里简略的描述本工程的使用：

1.基本信息：
本例程使用e-paper Driver HAT模块进行了验证，你可以在工程的Examples\中查看对应的测试例程;

2.管脚连接：
管脚连接你可以在\lib\Config目录下查看DEV_Config.h中查看，这里也再重述一次：
EPD    =>    Jetson Nano/RPI(BCM)
VCC    ->    3.3
GND    ->    GND
DIN    ->    10(SPI0_MOSI)
CLK    ->    11(SPI0_SCK)
CS     ->    8(SPI0_CS0)
DC     ->    25
RST    ->    17
BUSY   ->    24

3.基本使用：
由于本工程是一个综合工程，对于使用而言，你可能需要阅读以下内容：
你可以在main.c中看到许多测试函数和宏定义设置，在编译时指定屏幕对应的宏定义可以直接编译对应的测试程序。
请注意你购买的是哪一款的墨水屏。具体的对应关系可以查阅我们的Wiki和list.txt文件。
栗子1：
    如果你购买的3.7inch e-paper (V1)，那么你应该在编译时加上 EPD=epd3in7
        sudo make clean
        sudo make EPD=epd3in7
栗子2：
    如果你购买的2.9inch e-paper (B) (V1)，由于2.9寸的B型和C型是公用的驱动代码，那么你应该在编译时加上 EPD=epd2in9bc
        sudo make clean
        sudo make EPD=epd2in9bc
栗子3：
    如果你购买的是7.5inch e-Paper (V2)，那么你应该在编译时加上 EPD=epd7in5V2
        sudo make clean
        sudo make EPD=epd7in5V2

然后会生成可执行文件：epd
运行：sudo ./epd
如果修改了程序，需要执行：make clear，然后重新make。

4.目录结构（选读）：
如果你经常使用我们的产品，对我们的程序目录结构会十分熟悉，关于具体的函数的我们有一份
函数的API手册，你可以在我们的WIKI上下载或像售后客服索取，这里简单介绍一次：
\lib\Config\:此目录为硬件接口层文件，在DEV_Config.c(.h)可以看到很多定义，包括：
    数据类型;
    GPIO;
    读写GPIO;
    延时：注意：此延时函数并未使用示波器测量具体数值,因此会不准;
    模块初始化与退出的处理：
        void DEV_Module_Init(void);
        void DEV_Module_Exit(void);
        注意：1.这里是处理使用墨水屏前与使用完之后一些GPIO的处理。
              2.对于PCB带有Rev2.1的，DEV_Module_Exit()之后整个模块会进入低功耗，经过测试这个功耗基本为0;
              
\lib\GUI\:此目录为一些基本的图像处理函数，在GUI_Paint.c(.h)中：
    常用图像处理：创建图形、翻转图形、镜像图形、设置像素点、清屏等;
    常用画图处理：画点、线、框、圆、中文字符、英文字符、数字等;
    常用时间显示：提供一个常用的显示时间函数;
    常用显示图片：提供一个显示位图的函数;
    
\lib\Fonts\:为一些常用的字体：
    Ascii:
        font8: 5*8 
        font12: 7*12
        font16: 11*16 
        font20: 14*20 
        font24: 17*24
    中文：
        font12CN: 16*21 
        font24CN: 32*41
        
\lib\e-paper\:此目录下为墨水屏驱动函数;
Examples\:此目录下为墨水屏的测试程序，你可在其中看到具体的使用方法;