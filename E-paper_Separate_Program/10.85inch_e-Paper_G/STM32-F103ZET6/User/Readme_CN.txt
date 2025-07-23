/*****************************************************************************
* | File      	:   Readme_CN.txt
* | Author      :   Waveshare team
* | Function    :   Help with use
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2019-06-14
* | Info        :   �������ṩһ�����İ汾��ʹ���ĵ����Ա���Ŀ���ʹ��
******************************************************************************/
����ļ��ǰ�����ʹ�ñ����̡�
�������ǵ�īˮ��Խ��Խ�࣬���������ǵ�ά������˰����е�īˮ����������һ�����̡�
��������Ե����������̵�ʹ�ã�

1.������Ϣ��
�������ǻ���HAL����п����ģ���Ӧ��HAL�̼��汾Ϊ��STM32Cube_FW_F1_V1.6.1;
�������ǻ���STM32F103ZETx���п����ģ����̾������ǵ�Open103Z�������������֤;
������ʹ��e-paper Driver HATģ���������֤��������ڹ��̵�Examples\�в鿴��Ӧ�Ĳ�������;

2.�ܽ����ӣ�
�ܽ������������Config\Ŀ¼�²鿴DEV_Config.h�в鿴������Ҳ������һ�Σ�
EPD    =>    STM32f103Z
VCC    ->    3.3
GND    ->    GND
DIN    ->    PA7(MOSI)
CLK    ->    PA5(SCLK)
CS     ->    PA4
DC     ->    PA2
RST    ->    PA1
BUSY   ->    PA3

3.����ʹ�ã�
���ڱ�������һ���ۺϹ��̣�����ʹ�ö��ԣ��������Ҫ�Ķ��������ݣ�
�������main.c�е�73�е�97�п����Ѿ�������ע�͵�19��������
��ע���㹺�������һ���īˮ����
����1��
    ����㹺���5.83inch e-paper����ô��Ӧ�ðѶ�Ӧ��93�д����ע��ȥ��������
        //		EPD_5in83_test();
    �޸ĳɣ�
        EPD_5in83_test();
����2��
    ����㹺���2.9inch e-paper (B)������2.9���B�ͺ�C���ǹ��õ��������룬
    ��ô��Ӧ�ðѶ�Ӧ��82�д����ע��ȥ��������
        //		EPD_2in9bc_test();
    �޸ĳɣ�
        EPD_2in9bc_test();
ע�⣺����EPD_1in54_V2_test()��EPD_2in13_V2_test()��ע�������Ļ�����Ƿ�����V2��ʶ��

4.Ŀ¼�ṹ��ѡ������
����㾭��ʹ�����ǵĲ�Ʒ�������ǵĳ���Ŀ¼�ṹ��ʮ����Ϥ�����ھ���ĺ�����������һ��
������API�ֲᣬ����������ǵ�WIKI�����ػ����ۺ�ͷ���ȡ������򵥽���һ�Σ�
Config\:��Ŀ¼ΪӲ���ӿڲ��ļ�����DEV_Config.c(.h)���Կ����ܶඨ�壬������
    �������ͣ�
        #define UBYTE   uint8_t
        #define UWORD   uint16_t
        #define UDOUBLE uint32_t
    GPIO��
        #define EPD_RST_PIN     RST_GPIO_Port, RST_Pin
        #define EPD_DC_PIN      DC_GPIO_Port, DC_Pin
        #define EPD_CS_PIN      SPI_CS_GPIO_Port, SPI_CS_Pin
        #define EPD_BUSY_PIN    BUSY_GPIO_Port, BUSY_Pin
        ע�⣺����û�ж���DIN��CLK����Ϊ����Ӳ��SPI���Ĵ���
    ��дGPIO��
        #define DEV_Digital_Write(_pin, _value) HAL_GPIO_WritePin(_pin, _value == 0? GPIO_PIN_RESET:GPIO_PIN_SET)
        #define DEV_Digital_Read(_pin) HAL_GPIO_ReadPin(_pin)
    SPI�������ݣ�
        void DEV_SPI_WriteByte(UBYTE value);
    ��ʱ��
        #define DEV_Delay_ms(__xms) HAL_Delay(__xms);
        ע�⣺����ʱ������δʹ��ʾ��������������ֵ
    ģ���ʼ�����˳��Ĵ���
        void DEV_Module_Init(void);
        void DEV_Module_Exit(void);
        ע�⣺1.�����Ǵ���ʹ��īˮ��ǰ��ʹ����֮��һЩGPIO�Ĵ���
              2.����PCB����Rev2.1�ģ�DEV_Module_Exit()֮������ģ������͹��ģ���������������Ļ���Ϊ0;
              
GUI\:��Ŀ¼ΪһЩ������ͼ����������GUI_Paint.c(.h)�У�
    ����ͼ��������ͼ�Ρ���תͼ�Ρ�����ͼ�Ρ��������ص㡢������;
    ���û�ͼ�������㡢�ߡ���Բ�������ַ���Ӣ���ַ������ֵ�;
    ����ʱ����ʾ���ṩһ�����õ���ʾʱ�亯��;
    ������ʾͼƬ���ṩһ����ʾλͼ�ĺ���;
    
Fonts\:ΪһЩ���õ����壺
    Ascii:
        font8: 5*8 
        font12: 7*12
        font16: 11*16 
        font20: 14*20 
        font24: 17*24
    ���ģ�
        font12CN: 16*21 
        font24CN: 32*41
        
e-paper\:��Ŀ¼��Ϊīˮ����������;
Examples\:��Ŀ¼��Ϊīˮ���Ĳ��Գ�����������п��������ʹ�÷���;