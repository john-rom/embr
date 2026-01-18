cmake_minimum_required(VERSION 3.20.0)

project(thingy53_mic_impl)

find_package(Zephyr COMPONENTS unittest REQUIRED HINTS $ENV{ZEPHYR_BASE})

target_compile_definitions(testbinary PRIVATE UNIT_TEST)

target_sources(testbinary PRIVATE
  src/test_thingy53_mic_impl.c
  ../../../platform/src/thingy53_mic_impl.c
  ../mocks/thingy53_mic_specs_mock.c
)

target_include_directories(testbinary PRIVATE
  ../../../platform/include
  ../mocks
)
