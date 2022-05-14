set(SUBMODULE_NAME ${CMAKE_PROJECT_NAME}_core_commands)
set(${SUBMODULE_NAME}_SOURCES root.cpp)

add_library(${SUBMODULE_NAME} STATIC ${${SUBMODULE_NAME}_SOURCES})
target_link_libraries(${SUBMODULE_NAME} PUBLIC
        ${PROJECT_NAME}_cli_commands
        ${PROJECT_NAME}_multiformats
)

if (ENABLE_TESTING MATCHES ON)
    add_libs_for_testing(${SUBMODULE_NAME})
endif ()

# Including directories (IMPORTANT: path to includes is absolute)
include_directories(
        PUBLIC $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)
