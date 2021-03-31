import sys
from setuptools import setup

dependencies = ['Pillow', 'numpy']

if os.path.exists('/sys/bus/platform/drivers/gpiomem-bcm2835'):
    dependencies.extend(['RPi.GPIO', 'spidev'])
else:
    dependencies.extend(['Jetson.GPIO', 'smbus'])

setup(
    name='waveshare-epd',
    description='Waveshare e-Paper Display',
    author='Waveshare',
    package_dir={'': 'lib'},
    packages=['waveshare_epd'],
    install_requires=dependencies,
)

