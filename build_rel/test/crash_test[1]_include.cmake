if(EXISTS "/home/woo/bustub/build_rel/test/crash_test[1]_tests.cmake")
  include("/home/woo/bustub/build_rel/test/crash_test[1]_tests.cmake")
else()
  add_test(crash_test_NOT_BUILT crash_test_NOT_BUILT)
endif()
