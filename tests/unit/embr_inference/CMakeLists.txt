cmake_minimum_required(VERSION 3.20.0)

project(embr_inference)

find_package(Zephyr COMPONENTS unittest REQUIRED HINTS $ENV{ZEPHYR_BASE})

target_compile_definitions(testbinary PRIVATE UNIT_TEST)
target_compile_definitions(testbinary PRIVATE CONFIG_VM3011_PDM_BUFFER_SIZE=4000)

target_sources(testbinary PRIVATE
  src/test_embr_inference.c
  ../../../app/src/embr_inference.c
  ../mocks/ei_wrap_mock.c
  ../mocks/dmic_wrap_mock.c
  ../mocks/kernel_wrap_mock.c
  ../mocks/embr_app_mock.c
  ../mocks/thingy53_mic_mock.c
)

target_include_directories(testbinary PRIVATE
  ../../../app/include
  ../../../platform/include
  ../mocks
)
