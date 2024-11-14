add_library(fonts STATIC font_loader.cpp font_atlas.cpp font_component.cpp)
target_include_directories(fonts PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(fonts PUBLIC ui resource_management textures glad)