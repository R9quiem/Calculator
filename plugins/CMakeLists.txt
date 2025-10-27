# Папка, куда сложим .dll/.so/.dylib
set(PLUGINS_OUTPUT_DIR "${CMAKE_BINARY_DIR}/plugins")

function(add_calc_plugin NAME)
  add_library(${NAME} SHARED ${NAME}.cpp)

  target_link_libraries(${NAME} PRIVATE plugin_api)

  set_target_properties(${NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PLUGINS_OUTPUT_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${PLUGINS_OUTPUT_DIR}"
    ARCHIVE_OUTPUT_DIRECTORY "${PLUGINS_OUTPUT_DIR}"
  )
endfunction()

# Список плагинов

