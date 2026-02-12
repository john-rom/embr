cmake_minimum_required(VERSION 3.20.0)

project(embr_error_id)

find_package(Zephyr COMPONENTS unittest REQUIRED HINTS $ENV{ZEPHYR_BASE})

target_sources(testbinary PRIVATE
  src/test_embr_error_id.c
  ../../../app/src/embr_error_id.c
  ../mocks/kernel_wrap_mock.c
)

target_include_directories(testbinary PRIVATE
  ../../../app/include
  ../../../platform/include
  ../mocks
)
