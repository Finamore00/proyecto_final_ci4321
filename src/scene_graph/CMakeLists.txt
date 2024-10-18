add_library(scene_graph STATIC transform.cpp)
target_include_directories(scene_graph PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})