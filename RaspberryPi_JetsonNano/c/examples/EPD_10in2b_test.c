/*****************************************************************************
* | File      	:   EPD_10.2inb_test.c
* | Author      :   Waveshare team
* | Function    :   10.2inch b e-paper test demo
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2021-01-20
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
#include "EPD_10in2b.h"
#include <time.h> 

int EPD_10in2b_test(void)
{
    printf("EPD_10IN2b_test Demo\r\n");
    if(DEV_Module_Init()!=0){
        return -1;
    }

    printf("e-Paper Init and Clear...\r\n");
	EPD_10IN2b_Init();

	struct timespec start={0,0}, finish={0,0}; 
    clock_gettime(CLOCK_REALTIME,&start);
    EPD_10IN2b_Clear();
	clock_gettime(CLOCK_REALTIME,&finish);
    printf("%ld S\r\n",finish.tv_sec-start.tv_sec);	

    //Create a new image cache
    UBYTE *BlackImage, *RedImage;
    UDOUBLE Imagesize = ((EPD_10IN2b_WIDTH % 8 == 0)? (EPD_10IN2b_WIDTH / 8 ): (EPD_10IN2b_WIDTH / 8 + 1)) * EPD_10IN2b_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    if((RedImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for red memory...\r\n");
        return -1;
    }
    printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_10IN2b_WIDTH, EPD_10IN2b_HEIGHT, 0, WHITE);
	Paint_Clear(WHITE);
    printf("Paint_NewImage\r\n");
    Paint_NewImage(RedImage, EPD_10IN2b_WIDTH, EPD_10IN2b_HEIGHT, 0, WHITE);
	Paint_Clear(WHITE);
	
#if 1   // show bmp
	// Paint_NewImage(BlackImage, EPD_10IN2b_WIDTH, EPD_10IN2b_HEIGHT, 0, WHITE);
    printf("show window BMP-----------------\r\n");
    Paint_SelectImage(BlackImage);
    GUI_ReadBmp("./pic/10in2_b.bmp", 0, 0);

    printf("show bmp------------------------\r\n");
    Paint_SelectImage(RedImage);
    GUI_ReadBmp("./pic/10in2_r.bmp", 0, 0);
    EPD_10IN2b_Display(BlackImage, RedImage);
    DEV_Delay_ms(3000);
#endif

#if 1  // Drawing on the image	
    printf("Drawing\r\n");
    //1.Select Image
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
	
    // 2.Drawing on the image
    printf("Drawing:BlackImage\r\n");
    Paint_DrawPoint(10, 80, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);

    Paint_DrawLine(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);

    Paint_DrawRectangle(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    Paint_DrawCircle(45, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(105, 95, 20, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    Paint_DrawLine(85, 95, 125, 95, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);

    Paint_DrawString_EN(10, 10, "waveshare", &Font16, BLACK, WHITE);
    Paint_DrawString_EN(10, 30, "hello world", &Font12, WHITE, BLACK);

    Paint_DrawNum(10, 33, 123456789, &Font12, BLACK, WHITE);
    Paint_DrawNum(10, 50, 987654321, &Font16, WHITE, BLACK);

    Paint_DrawString_CN(130, 0,"ÄãºÃabc", &Font12CN, BLACK, WHITE);
    Paint_DrawString_CN(130, 20, "Î¢Ñ©µç×Ó", &Font24CN, WHITE, BLACK);
	

    Paint_DrawCircle(480, 480, 110, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);	
    Paint_DrawCircle(750, 270, 100, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
	Paint_DrawCircle(200, 310, 90, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
	
    Paint_DrawLine(200, 310, 480, 480, BLACK, DOT_PIXEL_3X3, LINE_STYLE_DOTTED);
    Paint_DrawLine(480, 480, 750, 270, BLACK, DOT_PIXEL_3X3, LINE_STYLE_DOTTED);

	// Red Image
    Paint_SelectImage(RedImage);
    Paint_Clear(WHITE);
    Paint_DrawLine(750, 270, 200, 310, BLACK, DOT_PIXEL_3X3, LINE_STYLE_SOLID);	
	
    Paint_DrawCircle(480, 480, 150, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);		
    Paint_DrawCircle(480, 480, 110, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawCircle(750, 270, 70, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);	
    Paint_DrawCircle(200, 310, 40, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
	
    EPD_10IN2b_Display(BlackImage, RedImage);
    DEV_Delay_ms(3000);
#endif

	
#if 1   // show bmp
    printf("show window BMP-----------------\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    GUI_ReadBmp("./pic/10in2_b1.bmp", 0, 0);

    printf("show bmp------------------------\r\n");
    Paint_SelectImage(RedImage);
    Paint_Clear(WHITE);
    GUI_ReadBmp("./pic/10in2_r1.bmp", 0, 0);
    EPD_10IN2b_Display(BlackImage, RedImage);
    DEV_Delay_ms(3000);
#endif

	printf("Clear...\r\n");
    EPD_10IN2b_Clear();
	
    printf("Goto Sleep...\r\n");
    EPD_10IN2b_Sleep();
    free(BlackImage);
    BlackImage = NULL;
    free(RedImage);
    RedImage = NULL;
    DEV_Delay_ms(2000);//important, at least 2s
    // close 5V
    printf("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();
    return 0;
}