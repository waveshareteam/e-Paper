/*****************************************************************************
* | File      	:   Readme_CN.txt
* | Author      :   Waveshare team
* | Function    :   Help with use
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2022-08-11
* | Info        :   在这里提供一个中文版本的使用文档，以便你的快速使用
******************************************************************************/
这个文件是帮助您使用本例程。
由于我们的墨水屏越来越多，不便于我们的维护，因此把所有的墨水屏程序做成一个工程。
在这里简略的描述本工程的使用：

1.基本信息：
本例程是基于Ubuntu 20.04 Server_20220718系统镜像而开发的
本例程是基于sunrise x3 pi进行开发的，例程均在sunrise x3 pi上进行了验证;
本例程使用e-paper Driver HAT模块进行了验证，你可以在工程的examples\中查看对应的测试例程;

2.管脚连接：
管脚连接你可以在\lib\epdconfig.py中查看，这里也再重述一次：
EPD    =>    sunrise x3 pi (BCM)
VCC    ->    3.3
GND    ->    GND
DIN    ->    10(SPI0_MOSI)
CLK    ->    11(SPI0_SCK)
CS     ->    8(SPI0_CS0)
DC     ->    25
RST    ->    17
BUSY   ->    24

3.安装库：
系统已经内置库，无需安装

4.基本使用：
由于本工程是一个综合工程，对于使用而言，你可能需要阅读以下内容：
你可以在examples\目录中查看测试程序
请注意你购买的是哪一款的墨水屏。
栗子1：
    如果你购买的5.83inch e-paper，那么你应该执行命令：
    sudo python epd_5in83_test.py
栗子2：
    如果你购买的2.9inch e-paper (B)，由于2.9寸的B型和C型是公用的驱动代码，
    那么你应该执行命令：
    sudo python epd_2in9bc_test.py
    
注意：对于epd_1in54_V2_test.py和epd_2in13_V2_test.py请注意你的屏幕背面是否贴有V2标识。
