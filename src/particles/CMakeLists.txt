add_library(particles STATIC particle_emitter.cpp emission_shape.cpp emission_time.cpp)
target_include_directories(particles PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(particles PUBLIC glad scene_graph shader)