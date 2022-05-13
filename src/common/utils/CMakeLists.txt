set(SUBMODULE_NAME ${CMAKE_PROJECT_NAME}_utils)

# Set headers and sources
set(${SUBMODULE_NAME}_SOURCES hex_util.cpp prefix_reader.cpp repo.cpp)

add_library(
        ${SUBMODULE_NAME} STATIC ${${SUBMODULE_NAME}_SOURCES}
)

find_package(Boost 1.79 REQUIRED COMPONENTS algorithm)
target_link_libraries(foo Boost::algorithm)

#target_link_libraries(
#        ${SUBMODULE_NAME} PUBLIC
#        ${CMAKE_PROJECT_NAME}_ds_fs)

############################################################
# Don't change the code below, just change SUBMODULE_NAME  #
# to yours and paste it into your submodule CMakeLists.txt #
############################################################

if (ENABLE_TESTING MATCHES ON)
    add_libs_for_testing(${SUBMODULE_NAME})
endif ()

# Including directories (IMPORTANT: path to includes is absolute)
include_directories(
        PUBLIC $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)
