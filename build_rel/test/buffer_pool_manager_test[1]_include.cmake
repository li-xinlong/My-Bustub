if(EXISTS "/home/woo/bustub/build_rel/test/buffer_pool_manager_test[1]_tests.cmake")
  include("/home/woo/bustub/build_rel/test/buffer_pool_manager_test[1]_tests.cmake")
else()
  add_test(buffer_pool_manager_test_NOT_BUILT buffer_pool_manager_test_NOT_BUILT)
endif()
