add_library(physics STATIC collision_primitives.cpp physic_engine.cpp)
target_include_directories(physics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(physics PUBLIC scene_graph)