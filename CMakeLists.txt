cmake_minimum_required(VERSION 3.2)

project(proyecto-1)
add_executable(proyecto-1 main.cpp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXECUTABLE_SUFFIX_CXX .o)
set(OpenGL_GL_PREFERENCE GLVND)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(thirdparty/glad)
add_subdirectory(thirdparty/stb)
add_subdirectory(thirdparty/tinyxml)
add_subdirectory(src/scene_graph)
add_subdirectory(src/rendering)
add_subdirectory(src/textures)
add_subdirectory(src/gl_utils)
add_subdirectory(src/physics)
add_subdirectory(src/resource_management)
add_subdirectory(src/mesh)
add_subdirectory(src/game)
add_subdirectory(src/input)
target_link_libraries(
    ${PROJECT_NAME} 
    PUBLIC 
    OpenGL::GL 
    glfw glad 
    shader 
    scene_graph 
    game 
    geometry 
    stb 
    textures 
    physics 
    rendering 
    input 
    tinyxml2 
    resource_management
    )