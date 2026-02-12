cmake_minimum_required(VERSION 3.20.0)

project(embr_classify)

find_package(Zephyr COMPONENTS unittest REQUIRED HINTS $ENV{ZEPHYR_BASE})

target_sources(testbinary PRIVATE
  src/test_embr_classify.c
  ../../../app/src/embr_classify.c
  ../mocks/ei_wrap_mock.c
)

target_include_directories(testbinary PRIVATE
  ../../../app/include
  ../mocks
)
