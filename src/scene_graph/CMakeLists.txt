add_library(SceneGraph STATIC transform.cpp)
target_include_directories(SceneGraph PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})