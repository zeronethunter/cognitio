cmake_minimum_required(VERSION 3.22)

project(gRPC_Test)

set(CMAKE_CXX_STANDARD 11)
find_package(gRPC REQUIRED)

set(TARGET_CLIENT RouteGuideClient)
set(TARGET_SERVER RouteGuideServer)
set(PROJECT_DIR project)
set(INCLUDE_DIR ${PROJECT_DIR}/include)
set(SOURCE_DIR ${PROJECT_DIR}/src)

add_executable(
        ${TARGET_CLIENT}
        ${PROJECT_DIR}/client.cpp
        ${SOURCE_DIR}/route_service.grpc.pb.cc
        ${SOURCE_DIR}/route_service.pb.cc
)
target_include_directories(${TARGET_CLIENT} PUBLIC ${INCLUDE_DIR})
target_link_libraries(${TARGET_CLIENT} PUBLIC gRPC::grpc++)


add_executable(
        ${TARGET_SERVER}
        ${PROJECT_DIR}/server.cpp
        ${SOURCE_DIR}/route_service.grpc.pb.cc
        ${SOURCE_DIR}/route_service.pb.cc
)

target_include_directories(${TARGET_SERVER} PUBLIC ${INCLUDE_DIR})
target_link_libraries(${TARGET_SERVER} gRPC::grpc++)
