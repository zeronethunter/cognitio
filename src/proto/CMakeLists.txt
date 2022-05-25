set(SUBMODULE_NAME ${CMAKE_PROJECT_NAME}_proto)
set(${SUBMODULE_NAME}_SOURCES ProtoConfig.pb.cc
  ProtoData.pb.cc api.grpc.pb.cc api.pb.cc
  kademlia.grpc.pb.cc kademlia.pb.cc )

find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)

add_library(${SUBMODULE_NAME} STATIC ${${SUBMODULE_NAME}_SOURCES})
target_link_libraries(
  ${SUBMODULE_NAME} PUBLIC
  ${Protobuf_LIBRARIES} gRPC::grpc++
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
