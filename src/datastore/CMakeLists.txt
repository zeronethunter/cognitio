set(SUBMODULE_NAME ${CMAKE_PROJECT_NAME}_ds_fs)
set(${SUBMODULE_NAME}_SOURCES ds_fs.cpp)

add_library(${SUBMODULE_NAME} STATIC ${${SUBMODULE_NAME}_SOURCES})
target_link_libraries(${SUBMODULE_NAME} PUBLIC
        ${PROJECT_NAME}_multiformats
        ${PROJECT_NAME}_common
)

if(ENABLE_TESTING MATCHES ON)
    add_libs_for_testing(${SUBMODULE_NAME})
endif()

include_directories(
        PUBLIC $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)
