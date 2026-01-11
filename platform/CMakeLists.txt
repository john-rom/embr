zephyr_library_named(embr_platform)

zephyr_library_include_directories(
  ${CMAKE_CURRENT_LIST_DIR}/include
)

zephyr_library_sources(
  src/gpio_wrap.c
  src/thingy53_led.c
  src/thingy53_led_impl.c
  src/thingy53_led_specs.c
)
