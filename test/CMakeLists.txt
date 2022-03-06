cmake_minimum_required(VERSION 3.15)

# Project details
project(
  ${CMAKE_PROJECT_NAME}Tests
  LANGUAGES C
)

message(STATUS "Adding tests under ${CMAKE_PROJECT_NAME}Tests...")

find_program(VALGRIND valgrind)
if (NOT VALGRIND)
  message(AUTHOR_WARNING "Valgrind wasn't found. Tests will be launched without it")
endif()

find_package(CMOCKA_LIB NAMES cmocka)
if (NOT CMOCKA_LIB)
  message(STATUS "CMocka not found. Fetching...")
  include(${CMAKE_SOURCE_DIR}/cmake/FetchCMocka.cmake)
  set(CMOKA_LIB cmocka-static)
endif()

# Setup code coverage if enabled
if (ENABLE_CODE_COVERAGE)
  target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC -O0 -g -fprofile-arcs -ftest-coverage)
  target_link_options(${CMAKE_PROJECT_NAME} PUBLIC -fprofile-arcs -ftest-coverage)
  message(STATUS "Code coverage is enabled and provided with GCC.")
elseif(VAGRIND)
  target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC -O0 -g)
endif()

foreach(file ${${CMAKE_PROJECT_NAME}_TEST_SOURCES})
  string(REGEX REPLACE "(.*/)([a-zA-Z0-9_ ]+)(\.c)" "\\2" test_name ${file})
  add_executable(${test_name}_Tests ${file})
  target_compile_features(${test_name}_Tests PUBLIC c_std_99)


  if(BUILD_EXECUTABLE)
    set(${CMAKE_PROJECT_NAME}_TEST_LIB ${CMAKE_PROJECT_NAME}_LIB)
  else()
    set(${CMAKE_PROJECT_NAME}_TEST_LIB ${CMAKE_PROJECT_NAME})
  endif()

  target_link_libraries( ${test_name}_Tests
    PUBLIC ${CMOCKA_LIB} ${${CMAKE_PROJECT_NAME}_TEST_LIB})

  # Add the unit tests
  if (VALGRIND)
    add_test(
      NAME ${test_name}_valgrind
      COMMAND ${VALGRIND} --error-exitcode=1 --read-var-info=yes
        --leak-check=full --show-leak-kinds=all ./${test_name}_Tests)

  else()
    add_test(
      NAME ${test_name}
      COMMAND ${test_name}_Tests)
  endif()

endforeach()

message(STATUS "Finished adding unit tests for ${CMAKE_PROJECT_NAME}.")
