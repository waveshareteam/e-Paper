/*****************************************************************************
* | File      	:   EPD_10in85g_test.c
* | Author      :   Waveshare team
* | Function    :   10.85inch e-paper (G) test demo
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2024-8-22
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
#include "EPD_Test.h"
#include "EPD_10in85g.h"
#include <time.h> 
#include <stdio.h>

int EPD_10in85g_test(void)
{

    Debug("EPD_10in85g_test Demo\r\n");
    if(DEV_Module_Init()!=0){
        return -1;
    }

    Debug("e-Paper Init and Clear...\r\n");
	EPD_10in85g_Init    ();

	struct timespec start={0,0}, finish={0,0}; 
    clock_gettime(CLOCK_REALTIME,&start);
    EPD_10in85g_Clear(EPD_10in85g_WHITE);
	clock_gettime(CLOCK_REALTIME,&finish);
    Debug("%ld S\r\n",finish.tv_sec-start.tv_sec);	

    //Create a new image cache
    UBYTE *Image;
    UDOUBLE Imagesize = ((EPD_10in85g_WIDTH % 4 == 0)? (EPD_10in85g_WIDTH / 4 ): (EPD_10in85g_WIDTH / 4 + 1)) * EPD_10in85g_HEIGHT;
    if((Image = (UBYTE *)malloc(Imagesize*2)) == NULL) {
        Debug("Failed to apply for black memory...\r\n");
        return -1;
    }
    Debug("Paint_NewImage\r\n");
    Paint_NewImage(Image, EPD_10in85g_WIDTH*2, EPD_10in85g_HEIGHT, 0, WHITE);
    Paint_SetScale(4);
    
#if 0  // show bmp
    Debug("show bmp------------------------\r\n");
    Paint_SelectImage(Image);
    GUI_ReadBmp_RGB_4Color("./pic/10in85G.bmp", 0, 0);
    EPD_10in85g_Display(Image);
    DEV_Delay_ms(2000);
#endif

#if 1   //show image for array    
    Debug("show image for array\r\n");
    EPD_10in85g_Display(gImage_10in85G);
    DEV_Delay_ms(2000);
#endif

#if 1 // Drawing on the image
    //1.Select Image
    printf("SelectImage:BlackImage\r\n");
    Paint_SelectImage(Image);
    Paint_Clear(EPD_10in85g_WHITE);

    // 2.Drawing on the image
    printf("Drawing:Image\r\n");
    Paint_DrawPoint(10, 80, EPD_10in85g_RED, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, EPD_10in85g_YELLOW, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, EPD_10in85g_BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawLine(20, 70, 70, 120, EPD_10in85g_RED, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, EPD_10in85g_RED, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawRectangle(20, 70, 70, 120, EPD_10in85g_YELLOW, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 120, EPD_10in85g_YELLOW, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawCircle(45, 95, 20, EPD_10in85g_BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(105, 95, 20, EPD_10in85g_BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawLine(85, 95, 125, 95, EPD_10in85g_RED, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, EPD_10in85g_YELLOW, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawString_EN(10, 0, "Red, yellow, white and black", &Font24, EPD_10in85g_RED, EPD_10in85g_YELLOW);
    Paint_DrawString_EN(10, 30, "Four color e-Paper", &Font16, EPD_10in85g_YELLOW, EPD_10in85g_BLACK);
    Paint_DrawString_CN(10, 125, "΢ѩ����", &Font24CN, EPD_10in85g_RED, EPD_10in85g_WHITE);
    Paint_DrawNum(10, 50, 123456, &Font12, EPD_10in85g_RED, EPD_10in85g_WHITE);

    printf("EPD_Display\r\n");
    EPD_10in85g_Display(Image);
    DEV_Delay_ms(3000);
#endif


#if 1   // Drawing on the image
    //1.Select Image
    printf("SelectImage:BlackImage\r\n");
    Paint_SelectImage(Image);
    Paint_Clear(EPD_10in85g_WHITE);

    int hNumber, hWidth, vNumber, vWidth;
    hNumber = 48;
	hWidth = EPD_10in85g_HEIGHT/hNumber; 
    vNumber = 48;
	vWidth = EPD_10in85g_WIDTH*2/vNumber;
    // 2.Drawing on the image
    printf("Drawing:BlackImage\r\n");
	for(int i=0; i<hNumber; i++) {  // horizontal
		Paint_DrawRectangle(1, 1+i*hWidth, EPD_10in85g_WIDTH*2, hWidth*(1+i), EPD_10in85g_BLACK + (i % 2), DOT_PIXEL_1X1, DRAW_FILL_FULL);
	}
	for(int i=0; i<vNumber; i++) {  // vertical
        if(i%2) {
            Paint_DrawRectangle(1+i*vWidth, 1, vWidth*(i+1), EPD_10in85g_HEIGHT, EPD_10in85g_YELLOW + (i/2%2), DOT_PIXEL_1X1, DRAW_FILL_FULL);
        }
	}

    printf("EPD_Display\r\n");
    EPD_10in85g_Display(Image);
    DEV_Delay_ms(3000);
#endif

    Debug("Clear...\r\n");
	EPD_10in85g_Init();
    EPD_10in85g_Clear(EPD_10in85g_WHITE);
	
    Debug("Goto Sleep...\r\n");
    EPD_10in85g_Sleep();
    free(Image);
    Image = NULL;
    DEV_Delay_ms(2000);//important, at least 2s
	
    // close 5V
    Debug("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();
    return 0;
}

