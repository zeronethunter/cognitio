add_subdirectory(block_storage)
add_subdirectory(pinner)
add_subdirectory(gc)

set(SUBMODULE_NAME ${CMAKE_PROJECT_NAME}_repo)
set(${SUBMODULE_NAME}_SOURCES repo.cpp repo_templ.cpp)

add_library(${SUBMODULE_NAME} STATIC ${${SUBMODULE_NAME}_SOURCES})
target_link_libraries(
        ${SUBMODULE_NAME} PUBLIC
        ${CMAKE_PROJECT_NAME}_block_storage
        ${CMAKE_PROJECT_NAME}_common
        ${CMAKE_PROJECT_NAME}_linked_data
)

if (ENABLE_TESTING MATCHES ON)
    add_libs_for_testing(${SUBMODULE_NAME})
endif ()

include_directories(
        PUBLIC $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)
