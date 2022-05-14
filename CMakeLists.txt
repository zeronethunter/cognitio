cmake_minimum_required(VERSION 3.15)

# Basic info
project(
  "cognitio"
  VERSION 0.1.0
  LANGUAGES CXX
)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR "In-source builds are not allowed")
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE DEBUG)
endif()

# Default settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(USE_ALT_NAMES "Use alternative names for the project" OFF)
option(GIT_SUBMODULE "Check submodules during build" ON)

if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
  # Default options
  option(ENABLE_CHECK_TOOLS "Enable format and code analysis tools" ON)
  option(WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)
  option(ENABLE_TESTING "Enable unit tests for the project" OFF)
  option(ENABLE_COVERAGE "Enable code coverage through GCC" OFF)
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

# Dependencies
add_subdirectory(third_party/SHA256)

# Configuring GCC flags
include(cmake/CompilerWarnings.cmake)
include(cmake/Sanitizers.cmake)

# Updating submodules
include(cmake/GitSubmodule.cmake)

# Setting libraries to link with tests (it's used for tests setting too)
include(cmake/GlobalVariables.cmake)

# Finding all files for the checking of static analyzers
file(GLOB_RECURSE ${PROJECT_NAME}_ALL_FILES ${PROJECT_SOURCE_DIR}/src *.cpp)
file(GLOB_RECURSE ${PROJECT_NAME}_ALL_FILES ${PROJECT_SOURCE_DIR}/include *.hpp)
#list(FILTER ${PROJECT_NAME}_ALL_FILES EXCLUDE REGEX "${PROJECT_SOURCE_DIR}/build|test/*")

# Clang-format, clang-tidy, cpplint and cppcheck configuration
include(cmake/FormatStaticAnalysys.cmake)

# Project building
add_subdirectory(src)

###############################################
# Install instructions will be written lately #
###############################################
#                 < ... >

# Installing project
# Setup alternative names
if(${PROJECT_NAME}_USE_ALT_NAMES MATCHES ON)
  string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWERCASE)
  string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPERCASE)
else()
  set(PROJECT_NAME_LOWERCASE ${PROJECT_NAME})
  set(PROJECT_NAME_UPPERCASE ${PROJECT_NAME})
endif()

# Unit testing setup
if(ENABLE_TESTING MATCHES ON)
  enable_testing()
  add_subdirectory(tests)
endif()
