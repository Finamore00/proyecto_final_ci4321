#include "input.hpp"

InputManager *InputManager::instance = nullptr;

InputManager *InputManager::get_instance() {
    if (instance == nullptr) {
        instance = new InputManager();
    }
    return instance;
}

void InputManager::set_window(GLFWwindow *window) {
    this->window = window;
}

bool InputManager::key_is_pressed(KeyCode key) {
    if (instance == nullptr) {
        return false;
    }

    return glfwGetKey(window, key) == GLFW_PRESS;
}