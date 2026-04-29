zephyr_library_named(embr_app)

zephyr_library_include_directories(
  ${CMAKE_CURRENT_LIST_DIR}/include
)

zephyr_library_sources(
  src/embr_app.c
  src/embr_app_logic.c
  src/embr_error_id.c
  src/embr_inference.c
  src/embr_classify.c
)

if(CONFIG_EDGE_IMPULSE)
  zephyr_library_sources(src/ei_wrap_real.c)
else()
  zephyr_library_sources(src/ei_wrap_stub.c)
endif()
