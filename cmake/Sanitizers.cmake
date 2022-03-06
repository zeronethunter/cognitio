# Memory, address, undefined and thread sanitizers
message(ENABLE_SANITIZERS = ${ENABLE_SANITIZERS})
if (ENABLE_SANITIZERS)

  target_compile_options(${PROJECT_NAME} PUBLIC -lasan)
  target_compile_options(${PROJECT_NAME} PUBLIC -fsanitize=leak)
  target_compile_options(${PROJECT_NAME} PUBLIC -fsanitize=undefined)
  target_compile_options(${PROJECT_NAME} PUBLIC -fno-omit-frame-pointer -fsanitize=address)

  target_link_options(${PROJECT_NAME} PUBLIC -lasan)
  target_link_options(${PROJECT_NAME} PUBLIC -fsanitize=leak)
  target_link_options(${PROJECT_NAME} PUBLIC -fsanitize=undefined)
  target_link_options(${PROJECT_NAME} PUBLIC -fno-omit-frame-pointer -fsanitize=address)

  message(STATUS "Sanitizers are enabled")
elseif(NOT ENABLE_COVERAGE AND NOT ENABLE_CHECK_TOOLS)
  target_compile_options(${PROJECT_NAME} PUBLIC "-O2")
endif()
