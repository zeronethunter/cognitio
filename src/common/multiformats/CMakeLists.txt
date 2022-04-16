set(SUBMODULE_NAME ${CMAKE_PROJECT_NAME}_common_multiformats)
set({SUBMODULE_NAME}_SOURCES cid.cpp multiaddr.cpp)

add_library(
    ${SUBMODULE_NAME} STATIC ${{SUBMODULE_NAME}_SOURCES}
)

if(ENABLE_TESTING MATCHES ON)
    add_libs_for_testing(${SUBMODULE_NAME})
endif()

include_directories(
  PUBLIC $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)