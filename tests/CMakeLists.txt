cmake_minimum_required(VERSION 3.15)

# Project details
project(
  ${CMAKE_PROJECT_NAME}_tests
  LANGUAGES CXX
)

message(STATUS "Adding tests under ${PROJECT_NAME}...")

if(ENABLE_VALGRIND MATCHES ON)
  find_program(VALGRIND valgrind)
  if(NOT VALGRIND)
    message(AUTHOR_WARNING "Valgrind wasn't found. Tests will be launched without it")
  endif()
else()
  message(STATUS "Tests will be launched without valgrind")
endif()

find_package(GTest)
if(NOT GTest_FOUND)
  message(STATUS "GTest not Found. Fetching it ...")

  include(FetchContent)
  fetchcontent_declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG 5f467ec04df33024e3c6760fa403b5cd5d8e9ace
  )

  fetchcontent_makeavailable(googletest)
endif()

# Getting all sources for tests
add_subdirectory(src)

# Setup code coverage if enabled
get_property(${CMAKE_PROJECT_NAME}_TEST_LIBS GLOBAL PROPERTY LIBS_LIST)
if(ENABLE_COVERAGE MATCHES ON)
  include(cmake/Coverage.cmake)
  foreach(TARGET_FOR_COVERAGE ${${CMAKE_PROJECT_NAME}_TEST_LIBS})
    target_coverage(${TARGET_FOR_COVERAGE})
  endforeach()
endif()

# Adding all test under valgrind if available
include_directories(${CMAKE_SOURCE_DIR}/include)
get_property(${CMAKE_PROJECT_NAME}_TEST_SOURCES GLOBAL PROPERTY TESTS_LIST)
foreach(FILE ${${CMAKE_PROJECT_NAME}_TEST_SOURCES})
  string(REGEX REPLACE "(.*/)([a-zA-Z0-9_ ]+)(\.cpp)" "\\2" TEST_NAME ${FILE})

  add_executable(${TEST_NAME} ${FILE})
  # target_include_directories(${TEST_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src>)

  target_link_libraries(
    ${TEST_NAME} PRIVATE GTest::GTest GTest::Main
    ${${CMAKE_PROJECT_NAME}_TEST_LIBS}
  )

  if(VALGRIND)
    add_test(
      NAME ${TEST_NAME}_valgrind
      COMMAND ${VALGRIND} --error-exitcode=1 --read-var-info=yes
      --leak-check=full --show-leak-kinds=all ./${TEST_NAME}
    )
  else()
    add_test(
      NAME ${TEST_NAME}
      COMMAND ./${TEST_NAME}
    )
  endif()
endforeach()

message(STATUS "Finished adding unit tests for ${CMAKE_PROJECT_NAME}.")
