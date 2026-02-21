import gpiod
import time

'''
This is a cleaner version of gpiotest_raw.py so you don't have to calculate line offset.
'''

def gpio_from_pin(pin):
    chip, line = PINMAP[pin]
    c = gpiod.Chip(chip)
    l = c.get_line(line)
    return c, l


PINMAP = {
    # --- Power / GND pins intentionally omitted ---
    # Header pin : (gpiochip, line)
    3:  ("gpiochip0", 63),  # GPIOA_14
    5:  ("gpiochip1", 5),   # GPIOA_15  ⚠ USB related
    7:  ("gpiochip1", 3),   # GPIOAO_3
    8:  ("gpiochip1", 0),   # GPIOAO_0
    10: ("gpiochip1", 1),   # GPIOAO_1
    11: ("gpiochip1", 2),   # GPIOAO_2
    12: ("gpiochip0", 74),  # GPIOX_9
    13: ("gpiochip0", 76),  # GPIOX_11
    16: ("gpiochip0", 75),  # GPIOX_10
    18: ("gpiochip0", 73),  # GPIOX_8

    19: ("gpiochip0", 20),  # GPIOH_4
    21: ("gpiochip0", 21),  # GPIOH_5
    22: ("gpiochip0", 48),  # GPIOC_7  (open-drain)
    23: ("gpiochip0", 23),  # GPIOH_7
    24: ("gpiochip0", 22),  # GPIOH_6

    27: ("gpiochip1", 3),   # GPIOAO_3 (same as pin 7)
    28: ("gpiochip1", 2),   # GPIOAO_2 (same as pin 11)

    32: ("gpiochip1", 4),   # GPIOAO_4
    34: ("gpiochip1", 8),   # GPIOAO_8
    36: ("gpiochip0", 24),  # GPIOH_8 ⚠ 5V open-drain
    37: ("gpiochip1", 9),   # GPIOAO_9
    38: ("gpiochip1", 10),  # GPIOAO_10 (not always bonded)
    40: ("gpiochip1", 11),  # GPIOAO_11
}

chip, line = gpio_from_pin(19)
line.request(consumer="app", type=gpiod.LINE_REQ_DIR_OUT)
line.set_value(1)

time.sleep(1)
line.set_value(0)
time.sleep(1)
line.release()
chip.close()