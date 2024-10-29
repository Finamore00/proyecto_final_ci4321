#pragma once
#include "../../thirdparty/glm/glm.hpp"
#include <GLFW/glfw3.h>

typedef int KeyCode;

class InputManager {
    public:
     static InputManager *get_instance();
     void set_window(GLFWwindow *window);
     bool key_is_pressed(KeyCode key);

    protected:
     GLFWwindow *window;
     static InputManager *instance;
     InputManager() = default;
};