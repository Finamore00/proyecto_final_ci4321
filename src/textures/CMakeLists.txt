add_library(textures STATIC texture.cpp)
target_include_directories(textures PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(textures PUBLIC glad stb)