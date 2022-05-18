add_subdirectory(utils)
add_subdirectory(logger)

set(SUBMODULE_NAME ${CMAKE_PROJECT_NAME}_common)
set(${SUBMODULE_NAME}_SOURCES status.cpp)

add_library(${SUBMODULE_NAME} STATIC ${${SUBMODULE_NAME}_SOURCES})
target_link_libraries(${SUBMODULE_NAME} PUBLIC
  ${CMAKE_PROJECT_NAME}_multiformats
  ${CMAKE_PROJECT_NAME}_logger
  ${CMAKE_PROJECT_NAME}_utils
)

if (ENABLE_TESTING MATCHES ON)
    add_libs_for_testing(${SUBMODULE_NAME})
endif ()

# Including directories (IMPORTANT: path to includes is absolute)
include_directories(
        PUBLIC $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)
