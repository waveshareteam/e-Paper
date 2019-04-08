#include <stdlib.h>     //exit()
#include <signal.h>     //signal()
#include <time.h>
#include "GUI_Paint.h"
#include "GUI_BMPfile.h"
#include "ImageData.h"
#include "EPD_2in13d.h"

void  Handler(int signo)
{
    //System Exit
    printf("\r\nHandler:Goto Sleep mode\r\n");
    EPD_Sleep();
    DEV_ModuleExit();

    exit(0);
}

int main(void)
{
    printf("2.13inch e-Paper D demo\r\n");
    DEV_ModuleInit();

    // Exception handling:ctrl + c
    signal(SIGINT, Handler);

    if(EPD_Init() != 0) {
        printf("e-Paper init failed\r\n");
    }
    EPD_Clear();
    DEV_Delay_ms(500);

    //Create a new image cache
    UBYTE *BlackImage;
    UWORD Imagesize = ((EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1)) * EPD_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        exit(0);
    }
    printf("size = %d\r\n", EPD_WIDTH / 8 * EPD_HEIGHT);
    printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_WIDTH, EPD_HEIGHT, 270, WHITE);
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
#if 1   // show bmp    
    printf("show bmp\r\n");
    Paint_SelectImage(BlackImage);
    
    Paint_Clear(WHITE);
    GUI_ReadBmp("./pic/100x100.bmp", 10, 0);
    EPD_Display(BlackImage);
    DEV_Delay_ms(500);

    Paint_Clear(WHITE);    
    GUI_ReadBmp("./pic/2in13d.bmp", 0, 0);
    EPD_Display(BlackImage);
    DEV_Delay_ms(2000);
#endif

#if 1  //show image for array    
    printf("show image for array\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_DrawBitMap(gImage_2in13d);
    
    EPD_Display(BlackImage);
    DEV_Delay_ms(2000);
#endif

#if 1   // Drawing on the image
    //1.Select Image
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);

    // 2.Drawing on the image    
    Paint_DrawString_EN(5, 5, "waveshare", &Font16, BLACK, WHITE);
    Paint_DrawNum(5, 25, 123456789, &Font16, BLACK, WHITE);
    Paint_DrawString_CN(5, 45,"ÄãºÃabcÊ÷Ý®ÅÉ", &Font12CN, BLACK, WHITE);
    Paint_DrawString_CN(5, 65,"Î¢Ñ©µç×Ó", &Font24CN, WHITE, BLACK);
    EPD_Display(BlackImage);
    DEV_Delay_ms(1000);
    
    Paint_Clear(WHITE);
    Paint_DrawPoint(5, 10, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(5, 25, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(5, 40, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawPoint(5, 55, BLACK, DOT_PIXEL_4X4, DOT_STYLE_DFT);
    Paint_DrawLine(20, 10, 70, 60, BLACK, LINE_STYLE_SOLID, DOT_PIXEL_1X1);
    Paint_DrawLine(70, 10, 20, 60, BLACK, LINE_STYLE_SOLID, DOT_PIXEL_1X1);
    Paint_DrawLine(170, 15, 170, 55, BLACK, LINE_STYLE_DOTTED, DOT_PIXEL_1X1);
    Paint_DrawLine(150, 35, 190, 35, BLACK, LINE_STYLE_DOTTED, DOT_PIXEL_1X1);
    Paint_DrawRectangle(20, 10, 70, 60, BLACK, DRAW_FILL_EMPTY, DOT_PIXEL_1X1);
    Paint_DrawRectangle(85, 10, 130, 60, BLACK, DRAW_FILL_FULL, DOT_PIXEL_1X1);
    Paint_DrawCircle(170, 35, 20, BLACK, DRAW_FILL_EMPTY, DOT_PIXEL_1X1);
    Paint_DrawCircle(170, 80, 20, BLACK, DRAW_FILL_FULL, DOT_PIXEL_1X1);
    EPD_Display(BlackImage);
    DEV_Delay_ms(1000);
#endif

#if 1   //Partial refresh, example shows time
    Paint_SelectImage(BlackImage);
    PAINT_TIME sPaint_time;
    struct tm *t;
    time_t tt;
    for (;;) {
        time(&tt);
        t = localtime(&tt);
        sPaint_time.Hour = t->tm_hour;
        sPaint_time.Min = t->tm_min;
        sPaint_time.Sec = t->tm_sec;
        Paint_ClearWindows(15, 80, 15 + Font20.Width * 7, 80 + Font20.Height, WHITE);
        Paint_DrawTime(15, 80, &sPaint_time, &Font20, WHITE, BLACK);
        EPD_DisplayPartial(BlackImage);
    }
#endif

    printf("Goto Sleep mode...\r\n");
    EPD_Sleep();
    free(BlackImage);
    BlackImage = NULL;

    return 0;
}
