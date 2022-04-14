# Check out the subdirectory template in the core folder
add_subdirectory(hello)
# ...

set(MAIN_BIN ${CMAKE_PROJECT_NAME})
set(${MAIN_BIN}_SOURCES main.cpp)

add_executable(${MAIN_BIN} ${${MAIN_BIN}_SOURCES})
set_target_properties(${MAIN_BIN} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

target_link_libraries(
  ${MAIN_BIN} PRIVATE
  ${CMAKE_PROJECT_NAME}_hello
)

include_directories(
  PUBLIC $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)
