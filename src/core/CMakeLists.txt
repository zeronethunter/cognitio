add_subdirectory(commands)
add_subdirectory(core_api)
add_subdirectory(context)

set(SUBMODULE_NAME ${CMAKE_PROJECT_NAME}_core)
# set(${SUBMODULE_NAME}_SOURCES core.cpp)

# add_library(${SUBMODULE_NAME} STATIC ${${SUBMODULE_NAME}_SOURCES})
target_link_libraries(${SUBMODULE_NAME} PUBLIC
        ${PROJECT_NAME}_core_commands
        ${PROJECT_NAME}_core_context
        ${PROJECT_NAME}_core_api
)

if (ENABLE_TESTING MATCHES ON)
    add_libs_for_testing(${SUBMODULE_NAME})
endif ()

# Including directories (IMPORTANT: path to includes is absolute)
include_directories(
        PUBLIC $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)
