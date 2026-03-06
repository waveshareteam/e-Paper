/*****************************************************************************
* | File      	:   Readme_CN.txt
* | Author      :   Waveshare team
* | Function    :   Help with use
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2019-06-20
* | Info        :   Here is an English version of the documentation for your quick use.
******************************************************************************/
This file is to help you use this Demo.
Since our ink screens are getting more and more, it is not convenient for our maintenance, so all the ink screen programs are made into one project.
A brief description of the use of this project is here:

1. Basic information:
This Demo was developed based on the 2019-04-08-raspbian-stretch system image;
This Demo has been verified on the Raspberry Pi 3B+;
This Demo has been verified using the e-paper Driver HAT module. 
You can view the corresponding test routines in the examples\ of the project;

2. Pin connection:
Pin connections can be viewed in \lib\epdconfig.py and will be repeated here:
EPD    =>    Jetson Nano/RPI(BCM)
VCC    ->    3.3
GND    ->    GND
DIN    ->    10(SPI0_MOSI)
CLK    ->    11(SPI0_SCK)
CS     ->    8(SPI0_CS0)
DC     ->    25
RST    ->    17
BUSY   ->    24

3.Installation library
    sudo apt-get update
    sudo apt-get install python-pip
    sudo apt-get install python-pil
    sudo pip install RPi.GPIO

or

    sudo apt-get update
    sudo apt-get install python3-pip
    sudo apt-get install python3-pil
    sudo pip3 install RPi.GPIO

4. Basic use:
Since this project is a comprehensive project, you may need to read the following for use:
You can view the test program in the examples\ directory.
Please note which ink screen you purchased.
Example 1:
     If you purchased 5.83inch e-paper, then you should execute the command:
     Sudo python epd_5in83_test.py
Example 2:
     If you buy a 2.9inch e-paper (B), since the 2.9-inch Type B and Type C are common driver codes,
     Then you should execute the command:
     Sudo python epd_2in9bc_test.py
    
Note: For epd_1in54_V2_test.py and epd_2in13_V2_test.py, please note that the V2 logo is attached to the back of your screen.
