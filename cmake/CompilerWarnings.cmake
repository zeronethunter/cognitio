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

if (WARNINGS_AS_ERRORS MATCHES ON)
   list(APPEND CLANG_WARNINGS -Werror)
endif()

set(GCC_WARNINGS
    ${CLANG_WARNINGS}
    -Wmisleading-indentation  # warn if indentation implies blocks where blocks do not exist

    $<$<CONFIG:DEBUG>:-ggdb3> # debug symbols
    $<$<CONFIG:DEBUG>:-O0>    # no optimization
    $<$<CONFIG:RELEASE>:-O3>  # maximum safe optimization
)

# Setting warnings
add_compile_options(${GCC_WARNINGS})
