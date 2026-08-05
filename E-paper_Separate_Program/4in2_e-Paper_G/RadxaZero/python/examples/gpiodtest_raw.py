import gpiod
import time

CHIP_NAME = "gpiochip0"
LINE_OFFSET = 20  # GPIO pin 19 gpio-532 = 532 - 512
'''
Note the first line gives the BASE GPIOs 512-596 so 512 is the base
Global GPIO number = base + line_offset.

TO test this connect LED +ve to GPIO pin 19 and -ve to GND (pin 6) of the radxa zero amlogic board.

$ sudo cat /sys/kernel/debug/gpio
gpiochip0: GPIOs 512-596, parent: platform/ff634400.bus:pinctrl@40, periphs-banks:
 gpio-532 (19 [GPIOH_4]        )
 gpio-533 (21 [GPIOH_5]        )
 gpio-534 (24 [GPIOH_6]        )
 gpio-535 (23 [GPIOH_7]        )
 gpio-536 (36 [GPIOH_8]        )
 gpio-549 (                    |reset               ) out hi ACTIVE LOW
 gpio-559 (                    |cd                  ) in  lo ACTIVE LOW
 gpio-560 (22 [GPIOC_7]        )
 gpio-575 (3 [GPIOA_14]        )
 gpio-583 (                    |reset               ) out hi ACTIVE LOW
 gpio-585 (18 [GPIOX_8]        )
 gpio-586 (12 [GPIOX_9]        )
 gpio-587 (16 [GPIOX_10]       )
 gpio-588 (13 [GPIOX_11]       )
 gpio-594 (                    |shutdown            ) out hi

gpiochip1: GPIOs 597-611, parent: platform/ff800000.bus:pinctrl@14, aobus-banks:
 gpio-597 (8 [GPIOAO_0]        )
 gpio-598 (10 [GPIOAO_1]       )
 gpio-599 (11,28 [GPIOAO_2]    )
 gpio-600 (7,27 [GPIOAO_3]     )
 gpio-601 (32 [GPIOAO_4]       )
 gpio-602 (5 [GPIOA_15]        )
 gpio-605 (35 [GPIOAO_8]       )
 gpio-606 (37 [GPIOAO_9]       )
 gpio-608 (40 [GPIOAO_11]      )
'''

def main():
    print(f"Testing GPIO pin {LINE_OFFSET} on {CHIP_NAME}")
    
    # Open GPIO chip
    chip = gpiod.Chip(CHIP_NAME)
    line = chip.get_line(LINE_OFFSET)
    
    # Request line as output
    line.request(
        consumer="gpiotest",
        type=gpiod.LINE_REQ_DIR_OUT
    )
    
    try:
        # Turn on GPIO pin 19
        print(f"Turning ON GPIO pin {LINE_OFFSET}")
        line.set_value(1)
        time.sleep(1)  # Wait 1 second
        
        # Turn off GPIO pin 19
        print(f"Turning OFF GPIO pin {LINE_OFFSET}")
        line.set_value(0)
        time.sleep(1)  # Wait 1 second
        
        print("GPIO test completed successfully")
    finally:
        # Release line and close chip
        line.release()
        chip.close()
        print("GPIO closed")

if __name__ == "__main__":
    main()
