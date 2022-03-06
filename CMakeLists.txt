cmake_minimum_required(VERSION 3.15)

# Basic info
project(
  "Template"
  VERSION 0.1.0
  LANGUAGES C)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR "In-source builds are not allowed")
endif()

# Default options
option(USE_ALT_NAMES "Use alternative names for the project" ON)
option(BUILD_EXECUTABLE "Build the project as an executable, not a library" ON)
option(ENABLE_CHECK_TOOLS "Enable format and code analysis tools" ON)
option(ENABLE_SANITIZERS "Memory, address, thread sanitizers" ON)
option(WARNING_AS_ERRORS "Treat compiler warnings as errors" ON)
option(ENABLE_TESTING "Enable unit tests for the project" ON)
option(ENABLE_COVERAGE "Enable code coverage through GCC" ON)
include(cmake/CheckToolsOptions.cmake)

# Default settings
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
if (ENABLE_COVERAGE OR ENABLE_CHECK_TOOLS)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

# All headers and implementation files
set(${PROJECT_NAME}_SOURCES ${CMAKE_SOURCE_DIR}/src/add.c)
set(${PROJECT_NAME}_HEADERS ${CMAKE_SOURCE_DIR}/include/add.h)
set(${PROJECT_NAME}_TEST_SOURCES ${CMAKE_SOURCE_DIR}/test/src/add_test.c)
set(${PROJECT_NAME}_EXE_SOURCES ${CMAKE_SOURCE_DIR}/src/main.c
                                ${${PROJECT_NAME}_SOURCES})

# Clang-format, clang-tidy, cpplint and cppcheck configuration
include(cmake/FormatStaticAnalysys.cmake)

# Project building
message(BUILD_EXECUTABLE = ${BUILD_EXECUTABLE})
if(BUILD_EXECUTABLE)
  add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_EXE_SOURCES})

  if(ENABLE_TESTING)
    # We will need the lib anyway in order to link it with the testing framework
    add_library(${PROJECT_NAME}_LIB ${${PROJECT_NAME}_HEADERS}
                                    ${${PROJECT_NAME}_SOURCES})
  endif()

else()
  add_library(${PROJECT_NAME} ${${PROJECT_NAME}_HEADERS}
                              ${${PROJECT_NAME}_SOURCES})
endif()

# Configuring GCC flags
include(cmake/Sanitizers.cmake)
include(cmake/CompilerWarnings.cmake)

# Identify and link with the specific "packages" the project uses
# find_package(package_name package_version REQUIRED package_type [other_options])
#target_link_libraries(
#  ${PROJECT_NAME}
#  PUBLIC
#    dependency1 ...
#  PRIVATE
#    dependency2 ...
#    ${PROJECT_NAME}_PROJECT_OPTIONS
#    ${PROJECT_NAME}_PROJECT_WARNINGS
#)
#
#if(${PROJECT_NAME}_BUILD_EXECUTABLE AND ${PROJECT_NAME}_ENABLE_UNIT_TESTING)
#  target_link_libraries(
#    ${PROJECT_NAME}_LIB
#    PUBLIC
#      dependency1 ...
#  )
#endif()

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
