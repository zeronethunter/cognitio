set(SUBMODULE_NAME ${CMAKE_PROJECT_NAME}_block_swap)
set(${SUBMODULE_NAME}_SOURCES block_swap.cpp client.cpp rpc/service.cpp)

add_library(${SUBMODULE_NAME} STATIC ${${SUBMODULE_NAME}_SOURCES})
target_link_libraries(${SUBMODULE_NAME} PUBLIC
        ${PROJECT_NAME}_grpc_client
        ${PROJECT_NAME}_kademlia
        ${PROJECT_NAME}_common
        ${PROJECT_NAME}_proto
        ${PROJECT_NAME}_repo
)

if (ENABLE_TESTING MATCHES ON)
    add_libs_for_testing(${SUBMODULE_NAME})
endif ()

include_directories(
        PUBLIC $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)
