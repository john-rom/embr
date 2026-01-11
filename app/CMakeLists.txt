zephyr_library_named(embr_app)

zephyr_library_include_directories(
  ${CMAKE_CURRENT_LIST_DIR}/include
)

zephyr_library_sources(
  src/embr_app.c
  src/embr_error_id.c
)
