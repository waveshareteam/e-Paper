import os

dependencies = ['Pillow']

if os.path.exists('/sys/bus/platform/drivers/gpiomem-bcm2835'):
    dependencies += ['RPi.GPIO', 'spidev']
elif os.path.exists('/sys/bus/platform/drivers/gpio-x3'):
    dependencies += ['Hobot.GPIO', 'spidev']
else:
    dependencies += ['Jetson.GPIO']

if __name__ == "__main__":
    print("\n".join(dependencies))
