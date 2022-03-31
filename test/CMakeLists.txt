cmake_minimum_required(VERSION 3.15)

# Project details
project(
  ${CMAKE_PROJECT_NAME}Tests
  LANGUAGES C
)

message(STATUS "Adding tests under ${CMAKE_PROJECT_NAME}Tests...")

if (ENABLE_VALGRIND MATCHES ON)
  find_program(VALGRIND valgrind)
  if (NOT VALGRIND)
    message(AUTHOR_WARNING "Valgrind wasn't found. Tests will be launched without it")
  endif()
else()
  message(STATUS "Tests will be launched without valgrind")
endif()

find_library(CMOCKA_LIB NAMES cmocka)
if (NOT CMOCKA_LIB)
  message(STATUS "CMocka not found. Fetching...")
  include(${CMAKE_SOURCE_DIR}/cmake/FetchCMocka.cmake)
  set(CMOKA_LIB cmocka-static)
endif()

# Setup code coverage if enabled
include(${CMAKE_SOURCE_DIR}/cmake/Coverage.cmake)
if (ENABLE_COVERAGE MATCHES ON)
  set(${CMAKE_PROJECT_NAME}_TEST_LIB ${CMAKE_PROJECT_NAME}_MATH_LIB)
  foreach(TARGET_FOR_COVERAGE ${${CMAKE_PROJECT_NAME}_TEST_LIB})
    target_coverage(${TARGET_FOR_COVERAGE})
  endforeach()
endif()

# Adding all test under valgrind if available
foreach(FILE ${${CMAKE_PROJECT_NAME}_TEST_SOURCES})
  string(REGEX REPLACE "(.*/)([a-zA-Z0-9_ ]+)(\.c)" "\\2" TEST_NAME ${FILE})
  add_executable(${TEST_NAME}_Tests ${FILE})
  target_compile_features(${TEST_NAME}_Tests PUBLIC c_std_99)

  target_link_libraries( ${TEST_NAME}_Tests
    PUBLIC ${CMOCKA_LIB} ${${CMAKE_PROJECT_NAME}_TEST_LIB})

  target_include_directories(${TEST_NAME}_Tests
    PUBLIC $<INSTALL_INTERFACE:include>
           $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    PRIVATE ${CMAKE_SOURCE_DIR}/src)

  # Add the unit tests
  if (VALGRIND)
    add_test(
      NAME ${TEST_NAME}_valgrind
      COMMAND ${VALGRIND} --error-exitcode=1 --read-var-info=yes
      --leak-check=full --show-leak-kinds=all ./${TEST_NAME}_Tests)
  else()
    add_test(
      NAME ${TEST_NAME}
      COMMAND ./${TEST_NAME}_Tests)
  endif()

endforeach()

message(STATUS "Finished adding unit tests for ${CMAKE_PROJECT_NAME}.")
