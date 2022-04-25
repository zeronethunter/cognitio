# Check out the subdirectory template in the core folder
# add_subdirectory(datastore)
# add_subdirectory(repo)
# ...

set(MAIN_BIN ${CMAKE_PROJECT_NAME})
set(${MAIN_BIN}_SOURCES main.cpp)

add_executable(${MAIN_BIN} ${${MAIN_BIN}_SOURCES})
set_target_properties(${MAIN_BIN} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_package(spdlog REQUIRED)
target_link_libraries( ${MAIN_BIN} PRIVATE spdlog::spdlog )

# target_link_libraries(
#         ${MAIN_BIN} PRIVATE
#         ${CMAKE_PROJECT_NAME}_ds_fs
#         ${CMAKE_PROJECT_NAME}_repo
#         ${CMAKE_PROJECT_NAME}_gc
#         ${CMAKE_PROJECT_NAME}_block_storage
#         ${CMAKE_PROJECT_NAME}_pin-manager
# )

include_directories(
        PUBLIC $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)
