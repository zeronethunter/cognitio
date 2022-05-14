add_subdirectory(cli)
add_subdirectory(common)
add_subdirectory(config)
add_subdirectory(core)
add_subdirectory(datastore)
add_subdirectory(exchange)
add_subdirectory(files)
add_subdirectory(linked_data)
add_subdirectory(proto)
add_subdirectory(repo)

# add_subdirectory(p2p)
# add_subdirectory(rpc)

set(MAIN_BIN ${CMAKE_PROJECT_NAME})
set(${MAIN_BIN}_SOURCES main.cpp)

add_executable(${MAIN_BIN} ${${MAIN_BIN}_SOURCES})
set_target_properties(${MAIN_BIN} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

target_link_libraries(
        ${MAIN_BIN} PRIVATE
        ${PROJECT_NAME}_core_commands
        ${PROJECT_NAME}_logger
        ${PROJECT_NAME}_cli
)

include_directories(
        PUBLIC $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)
