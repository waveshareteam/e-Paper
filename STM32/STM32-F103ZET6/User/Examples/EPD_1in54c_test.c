/*****************************************************************************
* | File      	:   EPD_1in54c_test.c
* | Author      :   Waveshare team
* | Function    :   1.54inch C e-paper test demo
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2019-06-12
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
#include "EPD_1in54c.h"

int EPD_test(void)
{
    printf("EPD_1IN54C_test Demo\r\n");
    DEV_Module_Init();

    printf("e-Paper Init and Clear...\r\n");
    EPD_1IN54C_Init();
    EPD_1IN54C_Clear();
    DEV_Delay_ms(500);

    //Create new image
    UBYTE *BlackImage, *YellowImage;
    UWORD Imagesize = ((EPD_1IN54C_WIDTH % 8 == 0)? (EPD_1IN54C_WIDTH / 8 ): (EPD_1IN54C_WIDTH / 8 + 1)) * EPD_1IN54C_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    if((YellowImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for red memory...\r\n");
        return -1;
    }
    printf("NewImage:BlackImage and YellowImage\r\n");
    Paint_NewImage(BlackImage, EPD_1IN54C_WIDTH, EPD_1IN54C_HEIGHT, 270, WHITE);
    Paint_NewImage(YellowImage, EPD_1IN54C_WIDTH, EPD_1IN54C_HEIGHT, 270, WHITE);

#if 1   //Drawing
    printf("Drawing------------------------\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_DrawPoint(5, 10, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(5, 25, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawLine(20, 10, 70, 60, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 10, 20, 60, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawRectangle(20, 10, 70, 60, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(85, 10, 135, 60, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    Paint_SelectImage(YellowImage);
    Paint_Clear(WHITE);
    Paint_DrawPoint(5, 40, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawPoint(5, 55, BLACK, DOT_PIXEL_4X4, DOT_STYLE_DFT);
    Paint_DrawLine(45, 10, 45, 60, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(20, 35, 70, 35, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawCircle(45, 35, 15, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(110, 35, 15, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawString_EN(5, 70, "waveshare", &Font20, BLACK, WHITE);
    Paint_DrawNum(5, 95, 123456789, &Font20, BLACK, WHITE);
    Paint_DrawString_CN(5, 120,"ÄãºÃabc", &Font12CN, BLACK, WHITE);

    EPD_1IN54C_Display(BlackImage, YellowImage);
    DEV_Delay_ms(5000);
#endif

#if 1  //show image for array
    printf("show image for array------------------------\r\n");
    Paint_SelectImage(BlackImage);
    Paint_DrawBitMap(gImage_1in54c_Black);

    Paint_SelectImage(YellowImage);
    Paint_DrawBitMap(gImage_1in54c_Yellow);

    EPD_1IN54C_Display(BlackImage, YellowImage);
    DEV_Delay_ms(5000);
#endif

    printf("Clear...\r\n");
    EPD_1IN54C_Clear();

    printf("Goto Sleep...\r\n");
    EPD_1IN54C_Sleep();
    free(BlackImage);
    free(YellowImage);
    BlackImage = NULL;
    YellowImage = NULL;

    // close 5V
    printf("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();
    
    return 0;
}

