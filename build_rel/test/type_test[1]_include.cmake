if(EXISTS "/home/woo/bustub/build_rel/test/type_test[1]_tests.cmake")
  include("/home/woo/bustub/build_rel/test/type_test[1]_tests.cmake")
else()
  add_test(type_test_NOT_BUILT type_test_NOT_BUILT)
endif()
