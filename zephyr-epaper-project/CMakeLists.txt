# E-Paper Driver Module
#
# Copyright (c) 2025
# SPDX-License-Identifier: Apache-2.0
#
# This is a Zephyr module. The actual build logic is in zephyr/CMakeLists.txt
# which is processed when this module is included via ZEPHYR_EXTRA_MODULES.
#
# Module structure follows Zephyr 4.2.1 standards:
# - zephyr/module.yml: Module metadata (points to cmake: zephyr)
# - zephyr/CMakeLists.txt: Module build logic
# - Kconfig: Module configuration options
# - dts/: Device tree bindings and board overlays
# - samples/: Sample applications

cmake_minimum_required(VERSION 3.20.0)

message(STATUS "Waveshare E-Paper Driver Module for Zephyr OS")
message(STATUS "This is a Zephyr module - include it via ZEPHYR_EXTRA_MODULES")
message(STATUS "See samples/epaper_demo/ for usage example")
