add_library(dummy_test STATIC dummy_test.cpp)
target_include_directories(dummy_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})