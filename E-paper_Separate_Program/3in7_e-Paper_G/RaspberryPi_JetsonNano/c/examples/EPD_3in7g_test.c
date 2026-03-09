/*****************************************************************************
* | File      	:   EPD_3in7g_test.c
* | Author      :   Waveshare team
* | Function    :   3inch7 e-paper (G) test demo
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2025-02-26
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
#include "EPD_3in7g.h"

int EPD_3in7g_test(void)
{
    printf("EPD_3IN7G_test Demo\r\n");
    if(DEV_Module_Init()!=0){
        return -1;
    }

    printf("e-Paper Init and Clear...\r\n");
    EPD_3IN7G_Init();
    EPD_3IN7G_Clear(EPD_3IN7G_WHITE); // White
    DEV_Delay_ms(2000);

    //Create a new image cache
    UBYTE *BlackImage;
    UWORD Imagesize = ((EPD_3IN7G_WIDTH % 4 == 0)? (EPD_3IN7G_WIDTH / 4 ): (EPD_3IN7G_WIDTH / 4 + 1)) * EPD_3IN7G_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_3IN7G_WIDTH, EPD_3IN7G_HEIGHT, 90, EPD_3IN7G_WHITE);
    Paint_SetScale(4);

#if 1   // show bmp
    EPD_3IN7G_Init_Fast();
    printf("show BMP-----------------\r\n");
    Paint_NewImage(BlackImage, EPD_3IN7G_WIDTH, EPD_3IN7G_HEIGHT, 0, EPD_3IN7G_WHITE);
    Paint_SetScale(4);
    Paint_SelectImage(BlackImage);
    GUI_ReadBmp_RGB_4Color("./pic/3.7inch-G.bmp", 0, 0);
    EPD_3IN7G_Display(BlackImage);
    DEV_Delay_ms(2000);
#endif

#if 0   // show bmp
    EPD_3IN7G_Init_Fast();
    printf("show BMP-----------------\r\n");
    EPD_3IN7G_Display(Image4color);
    DEV_Delay_ms(5000);
#endif

#if 1   // Drawing on the image
    //1.Select Image
    EPD_3IN7G_Init();
    printf("SelectImage:BlackImage\r\n");
    Paint_NewImage(BlackImage, EPD_3IN7G_WIDTH, EPD_3IN7G_HEIGHT, 90, EPD_3IN7G_WHITE);
    Paint_SetScale(4);
    Paint_SelectImage(BlackImage);
    Paint_Clear(EPD_3IN7G_WHITE);

    // 2.Drawing on the image
    printf("Drawing:BlackImage\r\n");
    Paint_DrawPoint(10, 80, EPD_3IN7G_RED, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, EPD_3IN7G_YELLOW, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, EPD_3IN7G_BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawLine(20, 70, 70, 120, EPD_3IN7G_RED, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, EPD_3IN7G_RED, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawRectangle(20, 70, 70, 120, EPD_3IN7G_YELLOW, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 120, EPD_3IN7G_YELLOW, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawCircle(45, 95, 20, EPD_3IN7G_BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(105, 95, 20, EPD_3IN7G_BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawLine(85, 95, 125, 95, EPD_3IN7G_RED, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, EPD_3IN7G_YELLOW, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawString_EN(10, 0, "Red,yellow,white and black", &Font16, EPD_3IN7G_RED, EPD_3IN7G_YELLOW);
    Paint_DrawString_EN(10, 20, "Four color e-Paper", &Font12, EPD_3IN7G_YELLOW, EPD_3IN7G_BLACK);
    Paint_DrawString_CN(150, 20, "Î¢Ñ©µç×Ó", &Font24CN, EPD_3IN7G_RED, EPD_3IN7G_WHITE);
    Paint_DrawNum(10, 35, 123456, &Font12, EPD_3IN7G_RED, EPD_3IN7G_WHITE);

    printf("EPD_Display\r\n");
    EPD_3IN7G_Display(BlackImage);
    DEV_Delay_ms(3000);
#endif

    Paint_NewImage(BlackImage, EPD_3IN7G_WIDTH, EPD_3IN7G_HEIGHT, 0, EPD_3IN7G_WHITE);
    Paint_SetScale(4);

#if 1   // Drawing on the image
    //1.Select Image
    printf("SelectImage:BlackImage\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(EPD_3IN7G_WHITE);

    // 2.Drawing on the image
    printf("Drawing:BlackImage\r\n");
    Paint_DrawRectangle(1, 1, EPD_3IN7G_WIDTH, EPD_3IN7G_HEIGHT/4, EPD_3IN7G_RED, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawRectangle(1, EPD_3IN7G_HEIGHT/4, EPD_3IN7G_WIDTH, EPD_3IN7G_HEIGHT/2, EPD_3IN7G_YELLOW, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawRectangle(1, EPD_3IN7G_HEIGHT/2, EPD_3IN7G_WIDTH, EPD_3IN7G_HEIGHT/4*3, EPD_3IN7G_BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    printf("EPD_Display\r\n");
    EPD_3IN7G_Display(BlackImage);
    DEV_Delay_ms(3000);
#endif

#if 1   // Drawing on the image
    //1.Select Image
    printf("SelectImage:BlackImage\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(EPD_3IN7G_WHITE);

    int hNumber, hWidth, vNumber, vWidth;
    hNumber = 20;
	hWidth = EPD_3IN7G_HEIGHT/hNumber;
    vNumber = 8;
	vWidth = EPD_3IN7G_WIDTH/vNumber;

    // 2.Drawing on the image
    printf("Drawing:BlackImage\r\n");
	for(int i=0; i<hNumber; i++) {  // horizontal
		Paint_DrawRectangle(1, 1+i*hWidth, EPD_3IN7G_WIDTH, hWidth*(1+i), EPD_3IN7G_BLACK + (i % 2), DOT_PIXEL_1X1, DRAW_FILL_FULL);
	}
	for(int i=0; i<vNumber; i++) {  // vertical
        if(i%2) {
            Paint_DrawRectangle(1+i*vWidth, 1, vWidth*(i+1), EPD_3IN7G_HEIGHT, EPD_3IN7G_YELLOW + (i/2%2), DOT_PIXEL_1X1, DRAW_FILL_FULL);
        }
	}
    printf("EPD_Display\r\n");
    EPD_3IN7G_Display(BlackImage);
    DEV_Delay_ms(3000);
#endif

    printf("Clear...\r\n");
    EPD_3IN7G_Init();
    EPD_3IN7G_Clear(EPD_3IN7G_WHITE);
    DEV_Delay_ms(2000);//important, at least 2s

    printf("Goto Sleep...\r\n");
    EPD_3IN7G_Sleep();
    free(BlackImage);
    BlackImage = NULL;
    DEV_Delay_ms(2000);//important, at least 2s
    // close 5V
    printf("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();
    
    return 0;
}

