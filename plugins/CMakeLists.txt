# Папка, куда сложим .dll/.so/.dylib
set(PLUGINS_OUTPUT_DIR "${CMAKE_BINARY_DIR}/plugins")

function(add_calc_plugin NAME)
  add_library(${NAME} MODULE ${NAME}.cpp)

  target_link_libraries(${NAME} PRIVATE plugin_api)

  set_target_properties(${NAME} PROPERTIES
    #убрать префикс "lib" перед dll
    PREFIX ""
    RUNTIME_OUTPUT_DIRECTORY "${PLUGINS_OUTPUT_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${PLUGINS_OUTPUT_DIR}"
    ARCHIVE_OUTPUT_DIRECTORY "${PLUGINS_OUTPUT_DIR}"
  )
endfunction()

# Список плагинов

add_calc_plugin(funcdeg)
add_calc_plugin(funcsin)
add_calc_plugin(funcln)
