function(add_sanitizer FLAG)
  message(STATUS "Adding -fsanitize=${FLAG}")
  add_compile_options(-fsanitize=${FLAG} -fno-omit-frame-pointer -lasan)
  add_link_options(-fsanitize=${FLAG} -fno-omit-frame-pointer -lasan)
endfunction()

# According to the GNU GCC docs:
# * address cannot be combined with thread
# * thread - address, leak
# * leak - thread
# * undefined - can be with all
# * memory - UNAVAILABLE in GCC (!!!)

if(ENABLE_ASAN)
  add_sanitizer(address)
endif()

if(ENABLE_LSAN)
  add_sanitizer(leak)
endif()

if(ENABLE_TSAN)
  add_sanitizer(thread)
endif()

if(ENABLE_UBSAN)
  add_sanitizer(undefined)
endif()
