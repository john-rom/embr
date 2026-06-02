cmake_minimum_required(VERSION 3.20.0)

project(embr_coap_client)

find_package(Zephyr COMPONENTS unittest REQUIRED HINTS $ENV{ZEPHYR_BASE})

target_sources(testbinary PRIVATE
  src/test_embr_coap_client.c
  ../../../app/src/embr_coap_client.c
  ../mocks/openthread_coap_wrap_mock.c
)

target_include_directories(testbinary PRIVATE
  ../../../app/include
  ../../../platform/include
  ../mocks/
)
