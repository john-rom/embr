cmake_minimum_required(VERSION 3.20.0)

project(embr_transport)

find_package(Zephyr COMPONENTS unittest REQUIRED HINTS $ENV{ZEPHYR_BASE})

target_sources(testbinary PRIVATE
  src/test_embr_transport.c
  ../../../app/src/embr_transport.c
  ../mocks/embr_coap_client_mock.c
)

target_include_directories(testbinary PRIVATE
  ../../../app/include
  ../mocks/
)
