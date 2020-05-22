#include <stdlib.h>     //exit()
#include <signal.h>     //signal()
#include "EPD_Test.h"   //Examples
#include "EPD_3in7.h"
void  Handler(int signo)
{
    //System Exit
    printf("\r\nHandler:exit\r\n");
    DEV_Module_Exit();

    exit(0);
}



int main(void)
{
    // Exception handling:ctrl + c
    signal(SIGINT, Handler);
    
    // EPD_1in02d_test();
    
    // EPD_1in54_test();
    // EPD_1in54_V2_test();
    // EPD_1in54b_test();
    // EPD_1in54c_test();

    // EPD_2in7_test();
    // EPD_2in7b_test();

    // EPD_2in9_test();
    // EPD_2in9bc_test();
    // EPD_2in9b_V2_test();
    // EPD_2in9d_test();

    // EPD_2in13_test();
    // EPD_2in13_V2_test();
    // EPD_2in13bc_test();
    // EPD_2in13b_V2_test();
    // EPD_2in13d_test();
    
    // EPD_3in7_test();
    
    // EPD_4in2_test();
    // EPD_4in2bc_test();
    // EPD_4in2b_V2_test();

    // EPD_5in83_test();
    // EPD_5in83bc_test();

    // EPD_7in5_test();
    // EPD_7in5_V2_test();
    // EPD_7in5bc_test();
    // EPD_7in5bc_V2_test();
    
    // EPD_7in5_HD_test();
    // EPD_7in5b_HD_test();
    

    
    return 0;
}
