# Template subdirectory CMakeLists.txt

# Configure all submodules and dependencies if needed
# add_subdirectory(proto_manager)
# ...

# If you need more then one cmake file
# include(cmake/<file_name>.cmake) # put them into cmake directory

# Set submodule name (this one will be accessible only in
# this file and subdirectories declared after this set)
# The first parameter can be whatever you want, the second
# one is strictly formatted for everyone's convenience:
# ${CMAKE_PROJECT_NAME}_the_path_to_submodule (path is in lowercase)
set(SUBMODULE_NAME ${CMAKE_PROJECT_NAME}_unixfs)

# Set headers and sources
set(${SUBMODULE_NAME}_SOURCES unixfs.cpp)

add_library(
        ${SUBMODULE_NAME} STATIC ${${SUBMODULE_NAME}_SOURCES}
)

# Identify and link with the specific libs the submodule uses
# No need to make PUBLIC interface when it's not supposed to be there
target_link_libraries(
        ${SUBMODULE_NAME} PUBLIC
        ${CMAKE_PROJECT_NAME}_common)

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
