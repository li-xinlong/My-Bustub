if(EXISTS "/home/woo/bustub/build_rel/test/clock_replacer_test[1]_tests.cmake")
  include("/home/woo/bustub/build_rel/test/clock_replacer_test[1]_tests.cmake")
else()
  add_test(clock_replacer_test_NOT_BUILT clock_replacer_test_NOT_BUILT)
endif()
