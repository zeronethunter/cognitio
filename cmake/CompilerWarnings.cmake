set(CLANG_WARNINGS
    -Wall
    -Wextra                   # reasonable and standard
    -Wshadow                  # warn the user if a variable declaration shadows one from a
                              # parent context
    -Wcast-align              # warn for potential performance problem casts
    -Wunused                  # warn on anything being unused
    -Wpedantic                # warn if non-standard C++ is used
    -Wconversion              # warn on type conversions that may lose data
    -Wsign-conversion         # warn on sign conversions
    -Wnull-dereference        # warn if a null dereference is detected
    -Wdouble-promotion        # warn if float is implicit promoted to double
    -Wformat=2                # warn on security issues around functions that format output
                              # (ie printf)
)

if (WARNINGS_AS_ERRORS)
  list(APPEND CLANG_WARNINGS -Werror)
endif()

set(GCC_WARNINGS
    ${CLANG_WARNINGS}
    -Wmisleading-indentation  # warn if indentation implies blocks where blocks do not exist
    -Wduplicated-cond         # warn if if / else chain has duplicated conditions
    -Wduplicated-branches     # warn if if / else branches have duplicated code
    -Wlogical-op              # warn about logical operations being used where bitwise were
                              # probably wanted
)

if(CMAKE_C_COMPILER_ID MATCHES ".*Clang")
  set(PROJECT_WARNINGS ${CLANG_WARNINGS})
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
  set(PROJECT_WARNINGS ${GCC_WARNINGS})
else()
  message(WARNING "No compiler warnings set for '${CMAKE_C_COMPILER_ID}' compiler.")
endif()

# Setting warnings
target_compile_options(${PROJECT_NAME} PUBLIC ${PROJECT_WARNINGS})
