add_library(geometry STATIC geometry.cpp mesh.cpp model.cpp model_loader.cpp)
target_include_directories(geometry PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(geometry PUBLIC assimp glad textures shader resource_management)