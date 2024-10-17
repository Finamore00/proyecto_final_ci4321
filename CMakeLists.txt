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
add_subdirectory(src/dummy_test)
add_subdirectory(src/utils)
add_subdirectory(src/scene_graph)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw OpenGL::GL glad dummy_test utils SceneGraph stb)