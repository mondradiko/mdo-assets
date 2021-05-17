# Copyright (c) 2021 Marceline Cramer
# SPDX-License-Identifier: MIT

cmake_minimum_required (VERSION 3.18)
project (mdo-assets VERSION 0.0.0)

# options
option (ENABLE_TESTS "Enable testing suite.")

# C standard
set (CMAKE_C_STANDARD 99)
set (CMAKE_C_STANDARD_REQUIRED TRUE)
add_compile_options (-Wpedantic)

# project includes
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}")

# main library
set (MDO_ASSETS_SRC
  src/allocator.c
)

add_library (mdo-assets STATIC ${MDO_ASSETS_SRC})

