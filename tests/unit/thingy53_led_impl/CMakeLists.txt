cmake_minimum_required(VERSION 3.20.0)

project(thingy53_led_impl)

find_package(Zephyr COMPONENTS unittest REQUIRED HINTS $ENV{ZEPHYR_BASE})

target_compile_definitions(testbinary PRIVATE UNIT_TEST)

target_sources(testbinary PRIVATE
  src/test_thingy53_led_impl.c
  ../../../platform/src/thingy53_led_impl.c
  ../mocks/gpio_wrap_mock.c
)

target_include_directories(testbinary PRIVATE
  ../../../platform/include
  ../mocks
)
