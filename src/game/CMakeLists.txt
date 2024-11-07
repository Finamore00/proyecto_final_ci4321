add_library(game STATIC bullet.cpp tank.cpp components/destroyable_component.cpp)
target_include_directories(game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/components")
target_link_libraries(game PUBLIC scene_graph)