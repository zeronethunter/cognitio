if(ENABLE_TESTING MATCHES ON)
  set_property(GLOBAL PROPERTY LIBS_LIST)
  set_property(GLOBAL PROPERTY TESTS_LIST)

  # accepts ARGV1 = global property name, ARGV2 ... ARGVN
  function(add_global)
    get_property(TMP GLOBAL PROPERTY ${ARGV0})
    foreach(ARG ${ARGV})
      if(NOT ${ARG} MATCHES ${ARGV0})
        set(TMP "${TMP};${ARG}")
      endif()
    endforeach()

    set_property(GLOBAL PROPERTY ${ARGV0} ${TMP})
  endfunction()

  function(add_libs_for_testing)
    add_global(LIBS_LIST ${ARGV})
  endfunction()

  function(add_test_sources)
    add_global(TESTS_LIST ${ARGV})
  endfunction()
endif()
