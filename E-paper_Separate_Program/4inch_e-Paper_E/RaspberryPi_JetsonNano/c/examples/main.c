#include <stdlib.h>     //exit()
#include <signal.h>     //signal()
#include "EPD_Test.h"   //Examples

void Handler(int signo)
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

// This reserved interface will be used for future screen upgrade versions. Simply add EPD=reserved after the compiler command
// reserved needs to be changed to its screen name
// #ifdef reserved
//     reserved_test(); 
// #else
//     printf("Please specify the EPD model when making. \r\n");
//     printf("Example: When you run the EPD_7in5_V2_test() program, input: sudo make clean && make EPD=epd7in5V2 \r\n");
//     printf("Don't know which program you need to run? Refer to the user manual (Wiki) and main.c \r\n");
// #endif

    EPD_4in0e_test(); 
    
    return 0;
}
