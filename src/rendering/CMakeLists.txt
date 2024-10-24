add_library(rendering STATIC rendering_engine.cpp)
target_include_directories(rendering PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(rendering PUBLIC glad scene_graph)