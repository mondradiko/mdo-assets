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
  src/asset_ctx.c
  src/decompressor.c
  src/raw_decompressor.c
)

add_library (mdo-assets STATIC ${MDO_ASSETS_SRC})

# dependencies
if (NOT PKG_CONFIG_FOUND)
  include (FindPkgConfig)
endif ()

if (NOT LIBUV_FOUND)
  pkg_check_modules (LIBUV libuv>=1.41.0 REQUIRED)
endif ()

# libraries
target_link_libraries (mdo-assets
  ${LIBUV_LIBRARIES}
)

# tests
if (ENABLE_TESTS)
  enable_testing ()
  add_subdirectory (tests)
endif ()
