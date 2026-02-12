cmake_minimum_required(VERSION 3.20.0)

project(embr_app_logic)

find_package(Zephyr COMPONENTS unittest REQUIRED HINTS $ENV{ZEPHYR_BASE})

target_sources(testbinary PRIVATE
  src/test_embr_app_logic.c
  ../../../app/src/embr_app_logic.c
)

target_include_directories(testbinary PRIVATE
  ../../../app/include
)
