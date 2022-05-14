set(SUBMODULE_NAME ${CMAKE_PROJECT_NAME}_linked_data)
set(${SUBMODULE_NAME}_SOURCES proto_block.cpp dag_node.cpp linked_data.cpp merkle_dag.cpp)

add_library(
        ${SUBMODULE_NAME} STATIC ${${SUBMODULE_NAME}_SOURCES}
)

target_link_libraries(${SUBMODULE_NAME} PUBLIC
        ${PROJECT_NAME}_proto
)

if (ENABLE_TESTING MATCHES ON)
    add_libs_for_testing(${SUBMODULE_NAME})
endif ()

# Including directories (IMPORTANT: path to includes is absolute)
include_directories(
        PUBLIC $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)
