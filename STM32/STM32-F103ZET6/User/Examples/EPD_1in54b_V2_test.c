/*****************************************************************************
* | File      	:   EPD_1in54b_test.c
* | Author      :   Waveshare team
* | Function    :   1.54inch B e-paper test demo
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2020-04-12
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
#include "EPD_1in54b_V2.h"  

int EPD_test(void)
{
    printf("EPD_1IN54b_V2_test Demo\r\n");
    if(DEV_Module_Init()!=0){
        return -1;
    }
    EPD_1IN54B_V2_Init();
    EPD_1IN54B_V2_Clear();
    DEV_Delay_ms(200);
    
    UBYTE *BlackImage, *RedImage;
    UWORD Imagesize = ((EPD_1IN54B_V2_WIDTH % 8 == 0)? (EPD_1IN54B_V2_WIDTH / 8 ): (EPD_1IN54B_V2_WIDTH / 8 + 1)) * EPD_1IN54B_V2_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    if((RedImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for red memory...\r\n");
        return -1;
    }
    printf("NewImage:BlackImage and RedImage\r\n");
    Paint_NewImage(BlackImage, EPD_1IN54B_V2_WIDTH, EPD_1IN54B_V2_HEIGHT, 90, WHITE);
    Paint_NewImage(RedImage, EPD_1IN54B_V2_WIDTH, EPD_1IN54B_V2_HEIGHT, 90, WHITE);

#if 1   //Drawing
    printf("Drawing------------------------\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_DrawPoint(5, 10, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(5, 25, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawLine(20, 10, 70, 60, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 10, 20, 60, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawRectangle(20, 10, 70, 60, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(170, 85, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawString_EN(5, 70, "hello world", &Font16, WHITE, BLACK);
    Paint_DrawString_CN(5, 155, "Î¢Ñ©µç×Ó", &Font24CN, WHITE, BLACK);

    Paint_SelectImage(RedImage);
    Paint_Clear(WHITE);
    Paint_DrawPoint(5, 40, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawPoint(5, 55, BLACK, DOT_PIXEL_4X4, DOT_STYLE_DFT);
    Paint_DrawLine(170, 15, 170, 55, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(150, 35, 190, 35, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawRectangle(85, 10, 130, 60, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawCircle(170, 35, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawString_EN(5, 90, "waveshare", &Font20, BLACK, WHITE);
    Paint_DrawNum(5, 120, 123456789, &Font20, BLACK, WHITE);
    Paint_DrawString_CN(5, 135,"ÄãºÃabc", &Font12CN, BLACK, WHITE);

    EPD_1IN54B_V2_Display(BlackImage, RedImage);
    DEV_Delay_ms(2000);
#endif

#if 1  //show image for array
    printf("show image for array------------------------\r\n");
    Paint_SelectImage(BlackImage);
    Paint_DrawBitMap(gImage_1in54b_Black);

    Paint_SelectImage(RedImage);
    Paint_DrawBitMap(gImage_1in54b_Red);

    EPD_1IN54B_V2_Display(BlackImage, RedImage);
    DEV_Delay_ms(2000);
#endif
    printf("Clear...\r\n");
    EPD_1IN54B_V2_Clear();

    printf("Goto Sleep...\r\n");
    EPD_1IN54B_V2_Sleep();
    free(BlackImage);
    free(RedImage);
    BlackImage = NULL;
    RedImage = NULL;

    // close 5V
    printf("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();
    return 0;
}

