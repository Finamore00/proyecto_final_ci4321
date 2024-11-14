add_library(ui STATIC dummy.cpp)
add_subdirectory(font)
target_include_directories(ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ui PUBLIC shader scene_graph)