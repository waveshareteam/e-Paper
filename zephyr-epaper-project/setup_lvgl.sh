#!/bin/bash
# Setup script for LVGL integration
# Copyright (c) 2025
# SPDX-License-Identifier: Apache-2.0

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
LVGL_DIR="$SCRIPT_DIR/lib/lvgl"

echo "=== LVGL Setup for Zephyr E-Paper Project ==="
echo ""

# Check if LVGL directory exists
if [ ! -d "$LVGL_DIR" ]; then
    echo "Error: LVGL library directory not found at $LVGL_DIR"
    exit 1
fi

cd "$LVGL_DIR"

# Check if LVGL is already cloned
if [ -d "lvgl" ]; then
    echo "LVGL library already exists at $LVGL_DIR/lvgl"
    echo ""
    read -p "Do you want to update it? (y/n) " -n 1 -r
    echo ""
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        echo "Updating LVGL..."
        cd lvgl
        git pull
        cd ..
    fi
else
    echo "Cloning LVGL library..."
    git clone https://github.com/lvgl/lvgl.git
    echo "LVGL cloned successfully!"
fi

echo ""
echo "=== Setup Complete! ==="
echo ""
echo "LVGL is now installed at: $LVGL_DIR/lvgl"
echo ""
echo "Next steps:"
echo "1. Connect your e-Paper display to your development board"
echo "2. Navigate to samples/epaper_demo"
echo "3. Build and flash:"
echo "   west build -b <your_board> -p"
echo "   west flash"
echo ""
echo "Supported boards:"
echo "  - nrf52840dk/nrf52840"
echo "  - nrf52dk/nrf52832"
echo "  - nrf54l15dk/nrf54l15/cpuapp"
echo ""
echo "Happy coding! 🎉"
