DIR_Config   = ./lib/Config
DIR_EPD      = ./lib/e-Paper
DIR_FONTS    = ./lib/Fonts
DIR_GUI      = ./lib/GUI
DIR_Examples = ./examples
DIR_BIN      = ./bin

OBJ_C = $(wildcard ${DIR_EPD}/*.c ${DIR_GUI}/*.c ${DIR_Examples}/*.c ${DIR_FONTS}/*.c )
OBJ_O = $(patsubst %.c,${DIR_BIN}/%.o,$(notdir ${OBJ_C}))
RPI_DEV_C = $(wildcard $(DIR_BIN)/dev_hardware_SPI.o $(DIR_BIN)/RPI_sysfs_gpio.o $(DIR_BIN)/DEV_Config.o )
JETSON_DEV_C = $(wildcard $(DIR_BIN)/sysfs_software_spi.o $(DIR_BIN)/sysfs_gpio.o $(DIR_BIN)/DEV_Config.o )


DEBUG = -D DEBUG

# USELIB_RPI = USE_BCM2835_LIB
USELIB_RPI = USE_WIRINGPI_LIB
# USELIB_RPI = USE_DEV_LIB

LIB_RPI=-Wl,--gc-sections
ifeq ($(USELIB_RPI), USE_BCM2835_LIB)
    LIB_RPI += -lbcm2835 -lm 
else ifeq ($(USELIB_RPI), USE_WIRINGPI_LIB)
    LIB_RPI += -lwiringPi -lm 
else ifeq ($(USELIB_RPI), USE_DEV_LIB)
    LIB_RPI += -lm 
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
CFLAGS += $(MSG)

RPI_epd:${OBJ_O}
	echo $(@)
	$(CC) $(CFLAGS) -D RPI $(OBJ_O) $(RPI_DEV_C) -o $(TARGET) $(LIB_RPI) $(DEBUG)
    
JETSON_epd:${OBJ_O}
	echo $(@)
	$(CC) $(CFLAGS) $(OBJ_O) $(JETSON_DEV_C) -o $(TARGET) $(LIB_JETSONI) $(DEBUG)

$(shell mkdir -p $(DIR_BIN))

${DIR_BIN}/%.o:$(DIR_Examples)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@ -I $(DIR_Config) -I $(DIR_GUI) -I $(DIR_EPD) $(DEBUG)
    
${DIR_BIN}/%.o:$(DIR_EPD)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@ -I $(DIR_Config) $(DEBUG)
    
${DIR_BIN}/%.o:$(DIR_FONTS)/%.c 
	$(CC) $(CFLAGS) -c  $< -o $@ $(DEBUG)
    
${DIR_BIN}/%.o:$(DIR_GUI)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@ -I $(DIR_Config) $(DEBUG)

RPI_DEV:
	$(CC) $(CFLAGS) $(DEBUG_RPI) -c  $(DIR_Config)/dev_hardware_SPI.c -o $(DIR_BIN)/dev_hardware_SPI.o $(LIB_RPI) $(DEBUG)
	$(CC) $(CFLAGS) $(DEBUG_RPI) -c  $(DIR_Config)/RPI_sysfs_gpio.c -o $(DIR_BIN)/RPI_sysfs_gpio.o $(LIB_RPI) $(DEBUG)
	$(CC) $(CFLAGS) $(DEBUG_RPI) -c  $(DIR_Config)/DEV_Config.c -o $(DIR_BIN)/DEV_Config.o $(LIB_RPI) $(DEBUG)
	
JETSON_DEV:
	$(CC) $(CFLAGS) $(DEBUG_JETSONI) -c  $(DIR_Config)/sysfs_software_spi.c -o $(DIR_BIN)/sysfs_software_spi.o $(LIB_JETSONI) $(DEBUG)
	$(CC) $(CFLAGS) $(DEBUG_JETSONI) -c  $(DIR_Config)/sysfs_gpio.c -o $(DIR_BIN)/sysfs_gpio.o $(LIB_JETSONI) $(DEBUG)
	$(CC) $(CFLAGS) $(DEBUG_JETSONI) -c  $(DIR_Config)/DEV_Config.c -o $(DIR_BIN)/DEV_Config.o $(LIB_JETSONI)  $(DEBUG)

clean :
	rm $(DIR_BIN)/*.* 
	rm $(TARGET) 

