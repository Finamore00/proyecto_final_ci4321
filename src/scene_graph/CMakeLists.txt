add_library(scene_graph STATIC transform.cpp)
target_include_directories(scene_graph PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(scene_graph PUBLIC geometry physics)