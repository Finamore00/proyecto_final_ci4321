add_library(scene_graph STATIC sceneobject.cpp transform.cpp component.cpp logic_engine.cpp)
target_include_directories(scene_graph PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(scene_graph PUBLIC geometry physics)