# Clang-format, clang-tidy, cpplint, cppcheck, scan-build, fbinfer
if (ENABLE_CHECK_TOOLS MATCHES ON)
  message(ENABLE_CHECK_TOOLS = ${ENABLE_CHECK_TOOLS})
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

  # Clang-format configuring
  if (ENABLE_CLANG_FORMAT MATCHES ON)
    find_program(CLANG_FORMAT_EXE clang-format)
    if (CLANG_FORMAT_EXE)
      set(CLANG_FORMAT_OPTS ${CLANG_FORMAT_EXE}
        -style=file)

      # Checking if the format is correct
      # The command doesn't work properly inside CMake. That's why we create this shell script
      set(CLANG_FORMAT_CHECK_SCRIPT formatcheck_script.sh)
      file(GENERATE OUTPUT ${CLANG_FORMAT_CHECK_SCRIPT} CONTENT "echo '${${PROJECT_NAME}_ALL_FILES}' |
              xargs -d ';' -I {} bash -c 'diff -u <(cat {}) <(${CLANG_FORMAT_EXE} {})'")
      add_custom_target(formatcheck
        COMMAND bash ${CLANG_FORMAT_CHECK_SCRIPT}
        COMMENT "Check the formatting of all files with clang-format")

      # Formatting code
      add_custom_target(formatcode
        COMMAND ${CLANG_FORMAT_OPTS} -i ${${PROJECT_NAME}_ALL_FILES}
        COMMENT "Format all files with clang-format")

      message(STATUS "Clang-format is available")
    else()
      message(AUTHOR_WARNING "Clang-format wasn't found. 'make clang-format' is unavailable")
    endif()
  endif()

  # CppCheck configuring
  if (ENABLE_CPPCHECK MATCHES ON)
    find_program(CMAKE_C_CPPCHECK cppcheck)
    if(CMAKE_C_CPPCHECK)
      list(APPEND CMAKE_C_CPPCHECK
        "--enable=warning,style,performance,portability"
        "--inconclusive" "--inline-suppr" "--force")

      add_custom_target(cppcheck
        COMMAND ${CMAKE_C_CPPCHECK} ${${PROJECT_NAME}_ALL_FILES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Static code analysis using cppcheck")
      message(STATUS "Cppcheck is enabled")

    else()
      message(AUTHOR_WARNING "Cppcheck is requested but executable wasn't found.")
    endif()
  endif()

  # Cpplint configuring
  if (ENABLE_CPPLINT MATCHES ON)
    find_program(CMAKE_C_CPPLINT cpplint)
    if (CMAKE_C_CPPLINT)

      # According to the presentation of D. Pddeifer header files
      # are not scanned during the compilation process.
      # You need to use 'make cpplint' for that
      list(APPEND CMAKE_C_CPPLINT
        "--recursive" "--extensions=c,cc,h,hh,cpp,hpp"
        "--filter=-build/include_subdir,-build/header_guard")

      add_custom_target(cpplint
        COMMAND ${CMAKE_C_CPPLINT} ${${PROJECT_NAME}_ALL_FILES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Static code analysis using cpplint")

      message(STATUS "Cpplint is enabled")

    else()
      message(AUTHOR_WARNING "Cpplint is requested but executable wasn't found.")
    endif()
  endif()

  if (ENABLE_CLANG_TIDY MATCHES ON)
    find_program(CMAKE_C_CLANG_TIDY clang-tidy)
    if (CMAKE_C_CLANG_TIDY)
      list(APPEND CMAKE_C_CLANG_TIDY
        "-p=${CMAKE_SOURCE_DIR}/build" "--format-style=file")

      add_custom_target(clang-tidy
        COMMAND ${CMAKE_C_CLANG_TIDY} ${${PROJECT_NAME}_ALL_FILES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Static code analysis using clang-tidy")

      message(STATUS "Clang-tidy is enabled")

    else()
      message(AUTHOR_WARNING "Clang-tidy is requested but executable wasn't found.")
    endif()
  endif()
endif()
