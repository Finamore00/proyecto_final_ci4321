add_library(input STATIC input.cpp)
target_include_directories(input PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(input PUBLIC glfw)