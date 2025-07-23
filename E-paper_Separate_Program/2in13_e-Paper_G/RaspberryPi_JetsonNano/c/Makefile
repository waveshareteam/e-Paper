DIR_Config	 = ./lib/Config
DIR_EPD		 = ./lib/e-Paper
DIR_FONTS	 = ./lib/Fonts
DIR_GUI		 = ./lib/GUI
DIR_Examples = ./examples
DIR_BIN		 = ./bin

EPD = NULL
ifeq ($(EPD), reserved)
# This reserved interface will be used for future screen upgrade versions. Simply add EPD=reserved after the compiler command
# reserved needs to be changed to its screen name
	OBJ_C_EPD = ${DIR_EPD}/reserved.c
	OBJ_C_Examples = ${DIR_Examples}/reserved_test.c
	OBJ_C = $(wildcard ${OBJ_C_EPD} ${DIR_GUI}/*.c ${OBJ_C_Examples} ${DIR_Examples}/main.c ${DIR_Examples}/ImageData2.c ${DIR_Examples}/ImageData.c ${DIR_FONTS}/*.c )
else ifeq ($(EPD), epd2in13g)
	OBJ_C_EPD = ${DIR_EPD}/EPD_2in13g.c
	OBJ_C_Examples = ${DIR_Examples}/EPD_2in13g_test.c
	OBJ_C = $(wildcard ${OBJ_C_EPD} ${DIR_GUI}/*.c ${OBJ_C_Examples} ${DIR_Examples}/main.c ${DIR_Examples}/ImageData2.c ${DIR_Examples}/ImageData.c ${DIR_FONTS}/*.c )
else 
	OBJ_C_EPD = ${DIR_EPD}/EPD_2in13g_V2.c
	OBJ_C_Examples = ${DIR_Examples}/EPD_2in13g_V2_test.c
	OBJ_C = $(wildcard ${OBJ_C_EPD} ${DIR_GUI}/*.c ${OBJ_C_Examples} ${DIR_Examples}/main.c ${DIR_Examples}/ImageData2.c ${DIR_Examples}/ImageData.c ${DIR_FONTS}/*.c )
endif

OBJ_O = $(patsubst %.c,${DIR_BIN}/%.o,$(notdir ${OBJ_C}))
RPI_DEV_C = $(wildcard $(DIR_BIN)/dev_hardware_SPI.o $(DIR_BIN)/RPI_sysfs_gpio.o $(DIR_BIN)/DEV_Config.o )
JETSON_DEV_C = $(wildcard $(DIR_BIN)/sysfs_software_spi.o $(DIR_BIN)/sysfs_gpio.o $(DIR_BIN)/DEV_Config.o )


DEBUG = -D DEBUG

# USELIB_RPI = USE_BCM2835_LIB
USELIB_RPI = USE_WIRINGPI_LIB
# USELIB_RPI = USE_LGPIO_LIB
# USELIB_RPI = USE_DEV_LIB

LIB_RPI=-Wl,--gc-sections
ifeq ($(USELIB_RPI), USE_BCM2835_LIB)
	LIB_RPI += -lbcm2835 -lm 
else ifeq ($(USELIB_RPI), USE_WIRINGPI_LIB)
	LIB_RPI += -lwiringPi -lm 
else ifeq ($(USELIB_RPI), USE_LGPIO_LIB)
	LIB_RPI += -llgpio -lm
else ifeq ($(USELIB_RPI), USE_DEV_LIB)
	LIB_RPI += -lgpiod -lm 
endif
DEBUG_RPI = -D $(USELIB_RPI) -D RPI

USELIB_JETSONI = USE_DEV_LIB
# USELIB_JETSONI = USE_HARDWARE_LIB
ifeq ($(USELIB_JETSONI), USE_DEV_LIB)
	LIB_JETSONI = -lm 
else ifeq ($(USELIB_JETSONI), USE_HARDWARE_LIB)
	LIB_JETSONI = -lm 
endif
DEBUG_JETSONI = -D $(USELIB_JETSONI) -D JETSON

.PHONY : RPI JETSON clean

RPI:RPI_DEV RPI_epd 
JETSON: JETSON_DEV JETSON_epd

TARGET = epd
CC = gcc
MSG = -g -O -ffunction-sections -fdata-sections -Wall
CFLAGS += $(MSG) -D $(EPD)

RPI_epd:${OBJ_O}
	echo $(@)
	$(CC) $(CFLAGS) -D RPI $(OBJ_O) $(RPI_DEV_C) -o $(TARGET) $(LIB_RPI) $(DEBUG)
	
JETSON_epd:${OBJ_O}
	echo $(@)
	$(CC) $(CFLAGS) $(OBJ_O) $(JETSON_DEV_C) -o $(TARGET) $(LIB_JETSONI) $(DEBUG)

$(shell mkdir -p $(DIR_BIN))

${DIR_BIN}/%.o:$(DIR_Examples)/%.c
	$(CC) $(CFLAGS) -c	$< -o $@ -I $(DIR_Config) -I $(DIR_GUI) -I $(DIR_EPD) $(DEBUG)
	
${DIR_BIN}/%.o:$(DIR_EPD)/%.c
	$(CC) $(CFLAGS) -c	$< -o $@ -I $(DIR_Config) $(DEBUG)

${DIR_BIN}/%.o:$(DIR_FONTS)/%.c 
	$(CC) $(CFLAGS) -c	$< -o $@ $(DEBUG)
	
${DIR_BIN}/%.o:$(DIR_GUI)/%.c
	$(CC) $(CFLAGS) -c	$< -o $@ -I $(DIR_Config) $(DEBUG)

RPI_DEV:
ifeq ($(USELIB_RPI), USE_DEV_LIB)
	$(CC) $(CFLAGS) $(DEBUG_RPI) -c	 $(DIR_Config)/dev_hardware_SPI.c -o $(DIR_BIN)/dev_hardware_SPI.o $(LIB_RPI) $(DEBUG)
	$(CC) $(CFLAGS) $(DEBUG_RPI) -c	 $(DIR_Config)/DEV_Config.c -o $(DIR_BIN)/DEV_Config.o $(LIB_RPI) $(DEBUG)
	$(CC) $(CFLAGS) $(DEBUG_RPI) -c	 $(DIR_Config)/RPI_gpiod.c -o $(DIR_BIN)/RPI_gpiod.o $(LIB_RPI) $(DEBUG)
else
	$(CC) $(CFLAGS) $(DEBUG_RPI) -c	 $(DIR_Config)/dev_hardware_SPI.c -o $(DIR_BIN)/dev_hardware_SPI.o $(LIB_RPI) $(DEBUG)
	$(CC) $(CFLAGS) $(DEBUG_RPI) -c	 $(DIR_Config)/DEV_Config.c -o $(DIR_BIN)/DEV_Config.o $(LIB_RPI) $(DEBUG)
endif
	
JETSON_DEV:
	$(CC) $(CFLAGS) $(DEBUG_JETSONI) -c	 $(DIR_Config)/sysfs_software_spi.c -o $(DIR_BIN)/sysfs_software_spi.o $(LIB_JETSONI) $(DEBUG)
	$(CC) $(CFLAGS) $(DEBUG_JETSONI) -c	 $(DIR_Config)/sysfs_gpio.c -o $(DIR_BIN)/sysfs_gpio.o $(LIB_JETSONI) $(DEBUG)
	$(CC) $(CFLAGS) $(DEBUG_JETSONI) -c	 $(DIR_Config)/DEV_Config.c -o $(DIR_BIN)/DEV_Config.o $(LIB_JETSONI)  $(DEBUG)

clean :
	rm $(DIR_BIN)/*.* 
	rm $(TARGET) 

