import sys
import os
picdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'pic')
libdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'lib')
if os.path.exists(libdir):
    sys.path.append(libdir)

import RADXAZero.GPIO as GPIO
import time

'''
This is a drop in replacement for Raspberry Pi style GPIO library for Radxa Zero Amlogic board.
previous versions include gpiodtest_hardcoded_pin_mapping.py and gpiotest_raw.py
To test this connect LED +ve to GPIO pin 19 and -ve to GND (pin 6) of the radxa zero amlogic board.
'''

PIN_TO_TEST = 19

GPIO.setmode(GPIO.BOARD)

GPIO.setup(PIN_TO_TEST, GPIO.OUT)
GPIO.output(PIN_TO_TEST, GPIO.HIGH)
time.sleep(1)
GPIO.output(PIN_TO_TEST, GPIO.LOW)

GPIO.cleanup()
