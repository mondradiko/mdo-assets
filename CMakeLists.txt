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

# dependencies
find_package (mdo-utils CONFIG REQUIRED)
find_package (lz4 CONFIG REQUIRED)
find_package (xxHash CONFIG REQUIRED)
find_package (unofficial-libuv CONFIG REQUIRED)

# setup library
include (mondradiko_setup_library)
mondradiko_setup_library (mdo-assets MDO_ASSETS_OBJ
  src/asset_ctx.c
  src/decompressor.c
  src/lump.c
  src/raw_decompressor.c
)

target_link_libraries (${MDO_ASSETS_OBJ} PUBLIC
  mondradiko::mdo-utils
  lz4::lz4
  xxHash::xxhash
  unofficial::libuv::libuv
)

# tests
if (ENABLE_TESTS)
  enable_testing ()
  add_subdirectory (tests)
endif ()
