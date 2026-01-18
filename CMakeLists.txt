cmake_minimum_required(VERSION 3.20.0)

list(APPEND ZEPHYR_EXTRA_MODULES ${CMAKE_CURRENT_SOURCE_DIR}/third_party/vm3011)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(ember)

zephyr_include_directories(
  app/include
  platform/include
)

target_sources(
  app PRIVATE src/main.c
)

add_subdirectory(app)
add_subdirectory(platform)

target_link_libraries(app PRIVATE
  embr_app
  embr_platform
)
