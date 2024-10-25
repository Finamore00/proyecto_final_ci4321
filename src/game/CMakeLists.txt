add_library(game STATIC bullet.cpp tank.cpp)
target_include_directories(game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(game PUBLIC scene_graph)