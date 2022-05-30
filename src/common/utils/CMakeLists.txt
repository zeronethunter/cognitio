set(SUBMODULE_NAME ${CMAKE_PROJECT_NAME}_utils)
set(${SUBMODULE_NAME}_SOURCES hex_util.cpp repo.cpp filesystem.cpp)

add_library(
        ${SUBMODULE_NAME} STATIC ${${SUBMODULE_NAME}_SOURCES}
)

# find_package(Boost 1.78 REQUIRED COMPONENTS algorithm)
find_package(Boost 1.74 REQUIRED)
# target_link_libraries($PRIVATE Boost::algorithm)

if (ENABLE_TESTING MATCHES ON)
    add_libs_for_testing(${SUBMODULE_NAME})
endif ()

include_directories(
        PUBLIC $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    include_directories(
        $<BUILD_INTERFACE:/System/Volumes/Data/opt/local/libexec/boost/1.76/include>
    )
endif()
