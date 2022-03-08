cmake_minimum_required(VERSION 3.15)

# Basic info
project(
  "Template"
  VERSION 0.1.0
  LANGUAGES C)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR "In-source builds are not allowed")
endif()

# Default settings
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_compile_definitions($<$<CONFIG:DEBUG>:DEBUG>)

if (CMAKE_BUILD_TYPE STREQUAL "DEBUG")
  # Default options
  option(USE_ALT_NAMES "Use alternative names for the project" ON)
  option(ENABLE_CHECK_TOOLS "Enable format and code analysis tools" ON)
  option(WARNINGS_AS_ERRORS "Treat compiler warnings as errors" ON)
  option(ENABLE_TESTING "Enable unit tests for the project" ON)
  option(ENABLE_COVERAGE "Enable code coverage through GCC" ON)
  option(ENABLE_VALGRIND "Launch all tests under valgrind" OFF)

  # ENABLE_CHECK_TOOLS turns on the default analyzers and formatters
  # The default layout can be changed in this file:
  include(cmake/CheckToolsOptions.cmake)

  # Sanitizers options
  # It's not a great idea to use sanitizers under Valgrind !!!
  # (The ones which are ON can be combined with each other)
  option(ENABLE_LSAN "Build with LeakSanitizer" ON)
  option(ENABLE_ASAN "Build with AdressSanitizer" ON)
  option(ENABLE_TSAN "Build with ThreadSanitizer" OFF)
  option(ENABLE_UBSAN "Build with UndefinedBehavior" ON)
endif()

# All headers and implementation files
set(${PROJECT_NAME}_MATH_LIB_SOURCES ${CMAKE_SOURCE_DIR}/src/math.c)
set(${PROJECT_NAME}_MATH_LIB_HEADERS ${CMAKE_SOURCE_DIR}/include/math.h)
set(${PROJECT_NAME}_TEST_SOURCES ${CMAKE_SOURCE_DIR}/test/src/math_test.c)
set(${PROJECT_NAME}_EXE_SOURCES ${CMAKE_SOURCE_DIR}/src/main.c)

# Don't forget to set this variable
# It is used for format and static analysis tools!
set(${PROJECT_NAME}_ALL_FILES
  ${${PROJECT_NAME}_EXE_SOURCES}
  ${${PROJECT_NAME}_MATH_LIB_HEADERS}
  ${${PROJECT_NAME}_MATH_LIB_SOURCES})

# Clang-format, clang-tidy, cpplint and cppcheck configuration
include(cmake/FormatStaticAnalysys.cmake)

# Configuring GCC flags
include(cmake/CompilerWarnings.cmake)
include(cmake/Sanitizers.cmake)

# Project building
add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_EXE_SOURCES})
add_library(${PROJECT_NAME}_MATH_LIB ${${PROJECT_NAME}_MATH_LIB_HEADERS}
  ${${PROJECT_NAME}_MATH_LIB_SOURCES})

# Identify and link with the specific "packages" or libs the project uses
# find_package(package_name package_version REQUIRED package_type [other_options])
target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC ${PROJECT_NAME}_MATH_LIB)

# Including directories
target_include_directories(
  ${PROJECT_NAME}
  PUBLIC $<INSTALL_INTERFACE:include>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

if(BUILD_EXECUTABLE AND ENABLE_TESTING)
  target_include_directories(
    ${PROJECT_NAME}_LIB
    PUBLIC $<INSTALL_INTERFACE:include>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
endif()


# Installing project
# Setup alternative names
if(${PROJECT_NAME}_USE_ALT_NAMES)
  string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWERCASE)
  string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPERCASE)
else()
  set(PROJECT_NAME_LOWERCASE ${PROJECT_NAME})
  set(PROJECT_NAME_UPPERCASE ${PROJECT_NAME})
endif()

# Install library for easy downstream inclusion
include(GNUInstallDirs)
install(
  TARGETS
  ${PROJECT_NAME}
  EXPORT
  ${PROJECT_NAME}Targets
  LIBRARY DESTINATION
  ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION
  ${CMAKE_INSTALL_BINDIR}
  ARCHIVE DESTINATION
  ${CMAKE_INSTALL_LIBDIR}
  INCLUDES DESTINATION
  include
  PUBLIC_HEADER DESTINATION
  include
)

install(
  EXPORT
  ${PROJECT_NAME}Targets
  FILE
  ${PROJECT_NAME}Targets.cmake
  NAMESPACE
  ${PROJECT_NAME}::
  DESTINATION
  ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(
  FILES
  ${CMAKE_CURRENT_BINARY_DIR}/include/${PROJECT_NAME_LOWERCASE}/version.hpp
  DESTINATION
  include/${PROJECT_NAME_LOWERCASE}
)

# Install the `include` directory
install(
  DIRECTORY
  include/${PROJECT_NAME_LOWERCASE}
  DESTINATION
  include
)

# Install with `cmake --build <build_directory> --target install --config <build_config>`
message(STATUS "Install targets succesfully build")

# Unit testing setup
if(ENABLE_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()
