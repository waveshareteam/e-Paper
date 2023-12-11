import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)

key1 = 5
key2 = 6
key3 = 13
key4 = 19

GPIO.setup(key1, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(key2, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(key3, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(key4, GPIO.IN, pull_up_down=GPIO.PUD_UP)


def interrupt(channel):
    print("interrupt", channel)


GPIO.add_event_detect(key1, GPIO.FALLING, callback=interrupt, bouncetime=200)
GPIO.add_event_detect(key2, GPIO.FALLING, callback=interrupt, bouncetime=200)
GPIO.add_event_detect(key3, GPIO.FALLING, callback=interrupt, bouncetime=200)
GPIO.add_event_detect(key4, GPIO.FALLING, callback=interrupt, bouncetime=200)

tic = 0
try:
    while True:
        tic = tic + 1
        print("Tic %d" % tic)
        time.sleep(1)
except KeyboardInterrupt:
    GPIO.cleanup()
    print("\nBye")
