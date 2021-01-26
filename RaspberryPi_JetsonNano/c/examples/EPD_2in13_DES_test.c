/*****************************************************************************
* | File      	:   EPD_2IN13_DES_test.c
* | Author      :   Waveshare team
* | Function    :   2.9inch DES e-paper test demo
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2020-01-13
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
#include "EPD_2in13_DES.h"
#include <time.h> 

int EPD_2in13_DES_test(void)
{
    printf("EPD_2IN13_DES_test Demo\r\n");
    if(DEV_Module_Init()!=0){
        return -1;
    }

    printf("e-Paper Init and Clear...\r\n");
	EPD_2IN13_DES_Init();

	struct timespec start={0,0}, finish={0,0}; 
    clock_gettime(CLOCK_REALTIME,&start);
    EPD_2IN13_DES_Clear();
	clock_gettime(CLOCK_REALTIME,&finish);
    printf("%ld S\r\n",finish.tv_sec-start.tv_sec);	

    //Create a new image cache
    UBYTE *BlackImage;
    UWORD Imagesize = ((EPD_2IN13_DES_WIDTH % 8 == 0)? (EPD_2IN13_DES_WIDTH / 8 ): (EPD_2IN13_DES_WIDTH / 8 + 1)) * EPD_2IN13_DES_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_2IN13_DES_WIDTH, EPD_2IN13_DES_HEIGHT, 90, WHITE);
	Paint_Clear(WHITE);
	
#if 0   // show bmp
	Paint_NewImage(BlackImage, EPD_2IN13_DES_WIDTH, EPD_2IN13_DES_HEIGHT, 90, WHITE);
    printf("show window BMP-----------------\r\n");
    Paint_SelectImage(BlackImage);
    GUI_ReadBmp("./pic/100x100.bmp", 10, 10);
    EPD_2IN13_DES_Display(BlackImage);
    DEV_Delay_ms(3000);

    printf("show bmp------------------------\r\n");
    Paint_SelectImage(BlackImage);
    GUI_ReadBmp("./pic/2in13.bmp", 0, 0);
    EPD_2IN13_DES_Display(BlackImage);
    DEV_Delay_ms(3000);
#endif

#if 0  //show image for array  
    Paint_NewImage(BlackImage, EPD_2IN13_DES_WIDTH, EPD_2IN13_DES_HEIGHT, 90, WHITE);  
    printf("show image for array\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_DrawBitMap(gImage_2in13);

    EPD_2IN13_DES_Display(BlackImage);
    DEV_Delay_ms(3000);
#endif

#if 1  // Drawing on the image
	Paint_NewImage(BlackImage, EPD_2IN13_DES_WIDTH, EPD_2IN13_DES_HEIGHT, 90, WHITE);  	
    printf("Drawing\r\n");
    //1.Select Image
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
	
    // 2.Drawing on the image
    printf("Drawing:BlackImage\r\n");
    Paint_DrawPoint(5, 10, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(5, 25, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(5, 40, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawPoint(5, 55, BLACK, DOT_PIXEL_4X4, DOT_STYLE_DFT);

    Paint_DrawLine(20, 10, 70, 60, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 10, 20, 60, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawRectangle(20, 10, 70, 60, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(85, 10, 135, 60, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    Paint_DrawLine(45, 15, 45, 55, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(25, 35, 65, 35, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawCircle(45, 35, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(110, 35, 20, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    Paint_DrawString_EN(140, 35, "wave", &Font16, BLACK, WHITE);
	Paint_DrawString_EN(140, 51, "share", &Font16, BLACK, WHITE);
    Paint_DrawNum(140, 75, 12345, &Font16, BLACK, WHITE);

    Paint_DrawString_CN(140, 10, "ÄãºÃabc", &Font12CN, BLACK, WHITE);
    Paint_DrawString_CN(5, 63, "Î¢Ñ©µç×Ó", &Font24CN, WHITE, BLACK);

    EPD_2IN13_DES_Display(BlackImage);
    DEV_Delay_ms(3000);
#endif

	printf("Clear...\r\n");
    EPD_2IN13_DES_Clear();
	
    printf("Goto Sleep...\r\n");
    EPD_2IN13_DES_Sleep();
	
    free(BlackImage);
    BlackImage = NULL;
    DEV_Delay_ms(2000);//important, at least 2s
    // close 5V
    printf("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();
    return 0;
}
