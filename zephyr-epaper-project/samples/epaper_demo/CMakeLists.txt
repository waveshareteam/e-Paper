# E-Paper Display Demo Application
#
# Copyright (c) 2025
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

# Note: When building this sample, you need to specify the module path.
# Option 1: Use west build with -DZEPHYR_EXTRA_MODULES
# Option 2: Set ZEPHYR_EXTRA_MODULES environment variable
# Option 3: Build from outside the module and add module to ZEPHYR_EXTRA_MODULES
#
# The module path should NOT be added here when the sample is inside the module,
# as it causes binary directory conflicts.

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(epaper_demo)

target_sources(app PRIVATE src/main.c)
