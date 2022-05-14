set(SUBMODULE_NAME ${CMAKE_PROJECT_NAME}_logger)
set(${SUBMODULE_NAME}_SOURCES logger.cpp)

add_library(${SUBMODULE_NAME} STATIC ${${SUBMODULE_NAME}_SOURCES})

find_package(spdlog REQUIRED)
target_link_libraries(${SUBMODULE_NAME} PUBLIC spdlog::spdlog)

if (ENABLE_TESTING MATCHES ON)
    add_libs_for_testing(${SUBMODULE_NAME})
endif ()

# Including directories (IMPORTANT: path to includes is absolute)
include_directories(
        PUBLIC $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)
