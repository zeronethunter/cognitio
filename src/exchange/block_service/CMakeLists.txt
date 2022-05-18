set(SUBMODULE_NAME ${CMAKE_PROJECT_NAME}_block_service)

# Set headers and sources
set(${SUBMODULE_NAME}_SOURCES block_service.cpp)

add_library(${SUBMODULE_NAME} STATIC ${${SUBMODULE_NAME}_SOURCES})
target_link_libraries(${SUBMODULE_NAME} PUBLIC
        ${CMAKE_PROJECT_NAME}_multiformats
        ${CMAKE_PROJECT_NAME}_linked_data
        ${CMAKE_PROJECT_NAME}_common
        ${CMAKE_PROJECT_NAME}_ds_fs
        ${CMAKE_PROJECT_NAME}_repo
)

if (ENABLE_TESTING MATCHES ON)
    add_libs_for_testing(${SUBMODULE_NAME})
endif ()

include_directories(
        PUBLIC $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)
