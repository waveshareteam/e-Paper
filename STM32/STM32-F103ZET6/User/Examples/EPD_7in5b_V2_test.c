/*****************************************************************************
* | File      	:   EPD_7in5b_V2_test.c
* | Author      :   Waveshare team
* | Function    :   7.5inch B V2 e-paper test demo
* | Info        :
*----------------
* |	This version:   V1.1
* | Date        :   2020-11-30
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
#include "EPD_7in5b_V2.h"

int EPD_test(void)
{
    printf("EPD_7IN5B_V2_test Demo\r\n");
    DEV_Module_Init();

    printf("e-Paper Init and Clear...\r\n");
    EPD_7IN5B_V2_Init();
    EPD_7IN5B_V2_Clear();
    DEV_Delay_ms(500);

    //Create a new image cache named IMAGE_BW and fill it with white
    UBYTE *Image;
    UWORD Imagesize = ((EPD_7IN5B_V2_WIDTH % 8 == 0)? (EPD_7IN5B_V2_WIDTH / 8 ): (EPD_7IN5B_V2_WIDTH / 8 + 1)) * EPD_7IN5B_V2_HEIGHT;
    if((Image = (UBYTE *)malloc(Imagesize / 2)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    printf("NewImage:Image\r\n");
    Paint_NewImage(Image, EPD_7IN5B_V2_WIDTH, EPD_7IN5B_V2_HEIGHT / 2, 0, WHITE);
    
    //Select Image
    Paint_SelectImage(Image);
    Paint_Clear(WHITE);

#if 1   // show image for array    
    printf("show image for array\r\n");
		//The entire image size is Imagesize 
		//Since the memory problem is transmitted halfway, now the other half is transmitted, so the offset address is required.
    EPD_7IN5B_V2_WritePicture(gImage_7in5_V2_b, 0);
    EPD_7IN5B_V2_WritePicture(gImage_7in5_V2_b + Imagesize/2, 1);
    EPD_7IN5B_V2_WritePicture(gImage_7in5_V2_ry, 2);
    EPD_7IN5B_V2_WritePicture(gImage_7in5_V2_ry + Imagesize/2, 3);
    DEV_Delay_ms(2000);
#endif

#if 1   // Drawing on the image
    /*Horizontal screen*/
    //1.Draw black image
    Paint_SelectImage(Image);
    Paint_Clear(WHITE);
    Paint_DrawPoint(10, 80, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 110, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawLine(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);      
    Paint_DrawRectangle(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawString_EN(10, 0, "waveshare", &Font16, BLACK, WHITE);    
    Paint_DrawString_CN(130, 20, "Î¢Ñ©µç×Ó", &Font24CN, WHITE, BLACK);
    Paint_DrawNum(10, 50, 987654321, &Font16, WHITE, BLACK);
    EPD_7IN5B_V2_WritePicture(Image, 0);
	Paint_Clear(WHITE);
	EPD_7IN5B_V2_WritePicture(Image, 1);
    //2.Draw red image
    Paint_Clear(WHITE);
    Paint_DrawCircle(160, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(210, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawLine(85, 95, 125, 95, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);  
    Paint_DrawString_CN(130, 0,"ÄãºÃabc", &Font12CN, BLACK, WHITE);
    Paint_DrawString_EN(10, 20, "hello world", &Font12, WHITE, BLACK);
    Paint_DrawNum(10, 33, 123456789, &Font12, BLACK, WHITE);
    EPD_7IN5B_V2_WritePicture(Image, 2);
	Paint_Clear(WHITE);
	EPD_7IN5B_V2_WritePicture(Image, 3);
    printf("EPD_Display\r\n");
    DEV_Delay_ms(2000);
#endif

    printf("Clear...\r\n");
    EPD_7IN5B_V2_Clear();

    printf("Goto Sleep...\r\n");
    EPD_7IN5B_V2_Sleep();
    free(Image);

    // close 5V
    printf("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();
    
    return 0;
}

