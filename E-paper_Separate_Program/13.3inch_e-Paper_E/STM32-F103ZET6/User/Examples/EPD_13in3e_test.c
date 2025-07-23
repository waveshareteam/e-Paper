#include "EPD_Test.h"
#include "EPD_13in3e.h"

int EPD_test(void)
{
    printf("13.3inch e-Paper E demo\r\n");
    DEV_Module_Init();

    EPD_13IN3E_Init();
    EPD_13IN3E_Clear(EPD_13IN3E_WHITE);
    DEV_Delay_ms(500);
    
    //Create a new image cache
    UBYTE *Image;
    /* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
    UDOUBLE Imagesize = ((EPD_13IN3E_WIDTH % 2 == 0)? (EPD_13IN3E_WIDTH / 2 ): (EPD_13IN3E_WIDTH / 2 + 1)) * EPD_13IN3E_HEIGHT;
    if((Image = (UBYTE *)malloc(Imagesize/32)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }

    printf("Paint_NewImage\r\n");
    Paint_NewImage(Image, 200, 300, 0, EPD_13IN3E_WHITE);
    Paint_SetScale(6);

#if 1 // show bmp
    EPD_13IN3E_DisplayPart(Image6color, 400, 500, 400, 600);
    DEV_Delay_ms(3000);
#endif
    
#if 1   // Drawing on the image
    Paint_NewImage(Image, 200, 300, 90, EPD_13IN3E_WHITE);
    Paint_SetScale(6);
    // 1.Select Image
    printf("SelectImage:BlackImage\r\n");
    Paint_SelectImage(Image);
    Paint_Clear(EPD_13IN3E_WHITE);

    // 2.Drawing on the image
    printf("Drawing:Image\r\n");
    Paint_DrawPoint(10, 80, EPD_13IN3E_RED, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, EPD_13IN3E_BLUE, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, EPD_13IN3E_GREEN, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawLine(20, 70, 70, 120, EPD_13IN3E_YELLOW, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, EPD_13IN3E_YELLOW, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawRectangle(20, 70, 70, 120, EPD_13IN3E_BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 120, EPD_13IN3E_BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawCircle(45, 95, 20, EPD_13IN3E_BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(105, 95, 20, EPD_13IN3E_WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawLine(85, 95, 125, 95, EPD_13IN3E_YELLOW, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, EPD_13IN3E_YELLOW, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawString_CN(10, 130, "ÄãºÃabc", &Font12CN, EPD_13IN3E_BLACK, EPD_13IN3E_WHITE);
    Paint_DrawString_CN(10, 150, "Î¢Ñ©µç×Ó", &Font24CN, EPD_13IN3E_WHITE, EPD_13IN3E_BLACK);
    Paint_DrawNum(10, 33, 123456789, &Font12, EPD_13IN3E_BLACK, EPD_13IN3E_WHITE);
    Paint_DrawNum(10, 50, 987654321, &Font16, EPD_13IN3E_WHITE, EPD_13IN3E_BLACK);
    Paint_DrawString_EN(145, 70, "Waveshare", &Font16, EPD_13IN3E_BLACK, EPD_13IN3E_WHITE);
    Paint_DrawString_EN(145, 90, "Waveshare", &Font16, EPD_13IN3E_GREEN, EPD_13IN3E_WHITE);
    Paint_DrawString_EN(145, 110, "Waveshare", &Font16, EPD_13IN3E_BLUE, EPD_13IN3E_WHITE);
    Paint_DrawString_EN(145, 130, "Waveshare", &Font16, EPD_13IN3E_RED, EPD_13IN3E_WHITE);
    Paint_DrawString_EN(145, 150, "Waveshare", &Font16, EPD_13IN3E_YELLOW, EPD_13IN3E_WHITE);

    printf("EPD_Display\r\n");
    EPD_13IN3E_DisplayPart(Image, 1000, 0, 200, 300);
    DEV_Delay_ms(3000);
#endif
    
    printf("Clear...\r\n");
    EPD_13IN3E_Clear(EPD_13IN3E_WHITE);

    printf("Goto Sleep...\r\n");
    EPD_13IN3E_Sleep();
    free(Image);
    Image = NULL;
    DEV_Delay_ms(2000); // important, at least 2s
    // close 5V
    printf("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();
    
    return 0;
}

