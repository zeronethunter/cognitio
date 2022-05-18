set(SUBMODULE_NAME ${CMAKE_PROJECT_NAME}_proto)
set(${SUBMODULE_NAME}_SOURCES ProtoConfig.pb.cc
  ProtoData.pb.cc ProtoResponseEmitter.pb.cc)

find_package(Protobuf REQUIRED)

add_library(${SUBMODULE_NAME} STATIC ${${SUBMODULE_NAME}_SOURCES})
target_link_libraries(
  ${SUBMODULE_NAME} PUBLIC
  ${Protobuf_LIBRARIES}
)

if (ENABLE_TESTING MATCHES ON)
    add_libs_for_testing(${SUBMODULE_NAME})
endif ()

# Including directories (IMPORTANT: path to includes is absolute)
include_directories(
        PUBLIC $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/proto>
        $<BUILD_INTERFACE:${Protobuf_INCLUDE_DIRS}>
)
