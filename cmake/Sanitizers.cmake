function(add_sanitizer FLAG)
  message(STATUS "Adding -fsanitize=${FLAG}")
  add_compile_options(-fsanitize=${FLAG} -fno-omit-frame-pointer)
  add_link_options(-fsanitize=${FLAG} -fno-omit-frame-pointer)

  # OMG. I literally hate this libasan with it's broken thread sanitizer
  if (${FLAG} STREQUAL "thread")
    execute_process(COMMAND ${CMAKE_C_COMPILER} -print-file-name=libasan.so
      OUTPUT_VARIABLE LIBASAN_PATH
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    set(ENV{LD_PRELOAD} ${LIBASAN_PATH})
  endif()
endfunction()

# According to the GNU GCC docs:
# * address cannot be combined with thread
# * thread - address, leak
# * leak - thread
# * undefined - can be with all
# * memory - UNAVAILABLE in GCC (!!!)

if(ENABLE_ASAN MATCHES ON)
  add_sanitizer(address)
endif()

if(ENABLE_LSAN MATCHES ON)
  add_sanitizer(leak)
endif()

if(ENABLE_TSAN MATCHES ON)
  add_sanitizer(thread)
endif()

if(ENABLE_UBSAN MATCHES ON)
  add_sanitizer(undefined)
endif()
