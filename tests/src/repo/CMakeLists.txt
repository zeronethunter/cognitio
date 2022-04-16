add_subdirectory(block_storage)
add_subdirectory(gc)
add_subdirectory(pinner)

add_test_sources(${CMAKE_CURRENT_SOURCE_DIR}/repo_test.cpp)
