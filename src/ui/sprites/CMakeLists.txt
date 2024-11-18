add_library(sprites STATIC sprite_component.cpp)
target_include_directories(fonts PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(sprites PUBLIC ui textures glad scene_graph)