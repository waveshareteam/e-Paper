#!/bin/bash
# Build script for e-Paper demo
# This script includes the correct module path

echo "Building e-Paper demo with module path..."
west build -b nrf52840dk/nrf52840 -p -- -DZEPHYR_EXTRA_MODULES=../..
