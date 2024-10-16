//
// Created by satnififu on 10/16/24.
//
#include "shader.h"

#include <iostream>
#include <string>
#include "../../thirdparty/glad/include/glad/glad.h"

namespace gl_utils {

    vertex_shader::vertex_shader(const std::string &src_path) {
        src_file = std::ifstream(src_path);
        if (!src_file.is_open()) {
            throw std::runtime_error("Error opening file " + src_path);
        }
        shader_id = glCreateShader(GL_VERTEX_SHADER);

        // Discuss if we want to compile at constructor-time or later.
        compile();
        if (check_compile_success(); !compile_status) {
            src_file.close();
            throw std::runtime_error("Error compiling vertex shader");
        }
    }

    void vertex_shader::compile() {
        const std::string shader_str = get_shader_str();
        const char *shader_cstr = shader_str.c_str();
        glShaderSource(shader_id, 1, &shader_cstr, nullptr);
        glCompileShader(shader_id);
    }

    bool vertex_shader::check_compile_success() {
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_status);
        if (compile_status == GL_FALSE) {
            glGetShaderInfoLog(shader_id, 512, nullptr, info_log);
            std::cerr << "Error compiling vertex shader." << std::endl;
            std::cerr << "Error log string: " << info_log << std::endl;
            return false;
        }
        return true;
    }

    std::string vertex_shader::get_shader_str() {
        std::string shader_str;
        std::string line;
        while (std::getline(src_file, line)) {
            shader_str += line + "\n";
        }

        return shader_str;
    }

    vertex_shader::~vertex_shader() {
        glDeleteShader(shader_id);
        src_file.close();
    }

    fragment_shader::fragment_shader(const std::string &file_path) {
        src_file = std::ifstream(file_path);
        if (!src_file.is_open()) {
            throw std::runtime_error("Error opening file " + file_path);
        }

        shader_id = glCreateShader(GL_FRAGMENT_SHADER);
        compile();
        if (check_compile_success(); !compile_status) {
            src_file.close();
            throw std::runtime_error("Error compiling fragment shader");
        }
    }

    void fragment_shader::compile() {
        const std::string shader_str = get_shader_str();
        const char *shader_cstr = shader_str.c_str();
        glShaderSource(shader_id, 1, &shader_cstr, nullptr);
        glCompileShader(shader_id);
    }

    bool fragment_shader::check_compile_success() {
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_status);
        if (compile_status == GL_FALSE) {
            glGetShaderInfoLog(shader_id, 512, nullptr, info_log);
            std::cerr << "Error compiling fragment shader." << std::endl;
            std::cerr << "Error log string: " << info_log << std::endl;
            return false;
        }

        return true;
    }

    std::string fragment_shader::get_shader_str() {
        std::string shader_str;
        std::string line;


        if (!src_file.is_open()) {
            std::cerr << "File isn't open" << std::endl;
            return "";
        }

        while (std::getline(src_file, line)) {
            shader_str += line + "\n";
        }

        return shader_str;
    }

    fragment_shader::~fragment_shader() {
        glDeleteShader(shader_id);
        src_file.close();
    }

}