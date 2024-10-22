#include <iostream>

#include "thirdparty/glad/include/glad/glad.h"
#include "thirdparty/glm/glm.hpp"
#include <GLFW/glfw3.h>

#include "src/gl_utils/shader.h"

// Window resize handler
void windowResizeCallback(GLFWwindow* window, int width, int height);
// Input processing
void processInput(GLFWwindow* window);
//Hex to float conversion
float hex_to_float(unsigned char hex);

int main()
{
    // Window initialization
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "CI4321 - Proyecto 1", nullptr, nullptr);
    if (window == nullptr)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, windowResizeCallback);

    // Glad initialization for buffer~memory handling.
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        glfwTerminate();
        return -1;
    }

    constexpr float vertices[] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f
    };

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Create shaders

    auto shader_program = gl_utils::shader_program(
        "../shader_files/color_triangle/shader.vert",
        "../shader_files/color_triangle/shader.frag");

    if (!shader_program.link_success()) {
        std::cerr << "Error linking shader programs" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    shader_program.use();

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // Main loop!
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(hex_to_float(0x36), hex_to_float(0x45), hex_to_float(0x4F), 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader_program.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

float hex_to_float(const unsigned char hex) {
    return static_cast<float>(hex) / static_cast<float>(0xFF);
}

void windowResizeCallback(GLFWwindow* window, const int width, const int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}