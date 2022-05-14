set(SUBMODULE_NAME ${CMAKE_PROJECT_NAME}_multiformats)
set(${SUBMODULE_NAME}_SOURCES cid.cpp multiaddr.cpp multihash.cpp)

add_library(${SUBMODULE_NAME} STATIC ${${SUBMODULE_NAME}_SOURCES})
target_link_libraries(${SUBMODULE_NAME} PRIVATE sha256)

if(ENABLE_TESTING MATCHES ON)
  add_libs_for_testing(${SUBMODULE_NAME})
endif()

include_directories(
  PUBLIC $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/third_party/SHA256/include>
)
