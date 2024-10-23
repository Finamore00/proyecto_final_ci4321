//
// Created by satnififu on 10/16/24.
//
#pragma once

#include <fstream>
#include <string>
#include "../../thirdparty/glad/include/glad/glad.h"
#include "../../thirdparty/glm/glm.hpp"

namespace gl_utils {

    class shader {

    public:
        GLuint shader_id;
        shader() = default;
        virtual ~shader() = default;
        virtual void compile() = 0;
        virtual std::string get_shader_str() = 0;
        virtual bool check_compile_success() = 0;

    protected:
        char info_log[512] {};
        GLint compile_status;
        std::ifstream src_file;
    };

    class vertex_shader final: public shader {
    public:
        vertex_shader() = default;
        explicit vertex_shader(const std::string& src_path);
        void compile() override;
        bool check_compile_success() override;
        std::string get_shader_str() override;
        ~vertex_shader() override;

    };

    class fragment_shader final: public shader {
    public:
        fragment_shader() = default;
        explicit fragment_shader(const std::string& file_path);
        void compile() override;
        bool check_compile_success() override;
        std::string get_shader_str() override;
        ~fragment_shader() override;
    };

    class shader_program final {
    public:
        GLuint program_id;
        shader_program(const std::string &vertex_path, const std::string &fragment_path);
        bool link_success();
        void set_mat4f(const std::string &name, const glm::mat4x4 &val) const;
        void use() const;
        ~shader_program();
    private:
        GLint link_status;
        char info_log[512] {};
    };

} // shader

