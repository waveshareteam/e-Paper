/*****************************************************************************
* | File      	:   EPD_2in9g.c
* | Author      :   Waveshare team
* | Function    :   2.9inch e-paper (G)
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2025-12-02
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
#include "EPD_3in6e.h"

int EPD_Test(void)
{
    printf("EPD_3IN6E_test Demo\r\n");
    DEV_Module_Init();

    Debug("e-Paper Init and Clear...\r\n");
    EPD_3IN6E_Init();
    EPD_3IN6E_Clear(EPD_3IN6E_WHITE); // WHITE  
    DEV_Delay_ms(500);

    //Create a new image cache
    UBYTE *BlackImage;
    UDOUBLE Imagesize = ((EPD_3IN6E_WIDTH % 2 == 0)? (EPD_3IN6E_WIDTH / 2 ): (EPD_3IN6E_WIDTH / 2 + 1)) * EPD_3IN6E_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize/4)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        while(1);
    }
    printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_3IN6E_WIDTH/2, EPD_3IN6E_HEIGHT/2, 0, EPD_3IN6E_WHITE);
    Paint_SetScale(6);

#if 1 // show bmp
    EPD_3IN6E_Init();
    printf("show bmp1-----------------\r\n");
    EPD_3IN6E_Display(BMP_1);
    DEV_Delay_ms(3000);
#endif

#if 1   // Drawing on the image
    EPD_3IN6E_Init();
    Paint_NewImage(BlackImage, EPD_3IN6E_WIDTH/2, EPD_3IN6E_HEIGHT/2, 90, EPD_3IN6E_WHITE);
    Paint_SetScale(6);
    // 1.Select Image
    printf("SelectImage:BlackImage\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(EPD_3IN6E_WHITE);

    // 2.Drawing on the image
    printf("Drawing:BlackImage\r\n");
    Paint_DrawPoint(10, 80, EPD_3IN6E_RED, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, EPD_3IN6E_BLUE, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, EPD_3IN6E_GREEN, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawLine(20, 70, 70, 120, EPD_3IN6E_YELLOW, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, EPD_3IN6E_YELLOW, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawRectangle(20, 70, 70, 120, EPD_3IN6E_BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 120, EPD_3IN6E_BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawCircle(45, 95, 20, EPD_3IN6E_BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(105, 95, 20, EPD_3IN6E_WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawLine(85, 95, 125, 95, EPD_3IN6E_YELLOW, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, EPD_3IN6E_YELLOW, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawString_CN(10, 130, "ÄãºÃabc", &Font12CN, EPD_3IN6E_BLACK, EPD_3IN6E_WHITE);
    Paint_DrawString_CN(10, 150, "Î¢Ñ©µç×Ó", &Font24CN, EPD_3IN6E_WHITE, EPD_3IN6E_BLACK);
    Paint_DrawNum(10, 33, 123456789, &Font12, EPD_3IN6E_BLACK, EPD_3IN6E_WHITE);
    Paint_DrawNum(10, 50, 987654321, &Font16, EPD_3IN6E_WHITE, EPD_3IN6E_BLACK);
    Paint_DrawString_EN(145, 0, "Waveshare", &Font24, EPD_3IN6E_WHITE, EPD_3IN6E_BLACK);
    Paint_DrawString_EN(145, 30, "Waveshare", &Font24, EPD_3IN6E_GREEN, EPD_3IN6E_BLACK);
    Paint_DrawString_EN(145, 60, "Waveshare", &Font24, EPD_3IN6E_BLUE, EPD_3IN6E_BLACK);
    Paint_DrawString_EN(145, 90, "Waveshare", &Font24, EPD_3IN6E_RED, EPD_3IN6E_BLACK);
    Paint_DrawString_EN(145, 120, "Waveshare", &Font24, EPD_3IN6E_YELLOW, EPD_3IN6E_BLACK);
    Paint_DrawString_EN(145, 150, "Waveshare", &Font24, EPD_3IN6E_BLACK, EPD_3IN6E_YELLOW);

    printf("EPD_Display\r\n");
    EPD_3IN6E_DisplayPart(BlackImage,200,0,200,300);
    DEV_Delay_ms(3000);
#endif

    printf("Clear...\r\n");
    EPD_3IN6E_Clear(EPD_3IN6E_WHITE);

    printf("Goto Sleep...\r\n");
    EPD_3IN6E_Sleep();
    free(BlackImage);
    BlackImage = NULL;
    DEV_Delay_ms(2000); // important, at least 2s
    // close 5V
    printf("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();
		
		return 0;
}

