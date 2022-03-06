option(ENABLE_CPPCHECK "")
option(ENABLE_CLANG_FORMAT "")
option(ENABLE_CPPLINT "")
option(ENABLE_CLANG_TIDY "")
option(ENABLE_SCAN_BUILD "")
option(ENABLE_FBINFER "")

# You can confure what you need
if (ENABLE_CHECK_TOOLS)
  # Clang-format will not be used automatically during
  # compilation for the sake of convenience.
  # Use 'make clang-format' instead
  set(ENABLE_CLANG_FORMAT OFF CACHE BOOL "" FORCE)
  set(ENABLE_CPPCHECK ON CACHE BOOL "" FORCE)
  set(ENABLE_CPPLINT ON CACHE BOOL "" FORCE)
  set(ENABLE_CLANG_TIDY ON CACHE BOOL "" FORCE)
endif()
