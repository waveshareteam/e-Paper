# LVGL Library CMakeLists.txt
#
# Copyright (c) 2025
# SPDX-License-Identifier: Apache-2.0

if(CONFIG_LVGL)
  zephyr_library_named(lvgl)

  # Add LVGL source directory (to be populated with LVGL library)
  # User should clone LVGL into lib/lvgl/lvgl directory
  set(LVGL_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lvgl)

  if(EXISTS ${LVGL_ROOT_DIR})
    message(STATUS "LVGL library found at: ${LVGL_ROOT_DIR}")

    # LVGL core sources
    file(GLOB_RECURSE LVGL_SOURCES
      ${LVGL_ROOT_DIR}/src/*.c
    )

    zephyr_library_sources(${LVGL_SOURCES})
    zephyr_library_include_directories(${LVGL_ROOT_DIR})
    zephyr_library_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

    # Compile definitions
    zephyr_library_compile_definitions(LV_CONF_INCLUDE_SIMPLE)

  else()
    message(WARNING "LVGL library not found. Please clone LVGL to ${LVGL_ROOT_DIR}")
    message(WARNING "Run: git clone https://github.com/lvgl/lvgl.git ${LVGL_ROOT_DIR}")
  endif()

  # Add e-Paper display driver for LVGL
  if(CONFIG_LVGL_EPAPER_DRIVER)
    zephyr_library_sources(lvgl_epaper_driver.c)
  endif()

endif()
