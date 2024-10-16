//
// Created by satnififu on 10/16/24.
//
#pragma once

#include <fstream>
#include <string>
#include "../../thirdparty/glad/include/glad/glad.h"

namespace gl_utils {

    class shader {

    public:
        GLuint shader_id;
        shader() = default;
        virtual ~shader() = default;
        virtual void compile() {};
        virtual std::string get_shader_str() {};
        virtual bool check_compile_success() {return false;};

    protected:
        char info_log[512] {};
        GLint compile_status;
        std::ifstream src_file;
    };

    class vertex_shader final: public shader {
    public:
        explicit vertex_shader(const std::string& src_path);
        void compile() override;
        bool check_compile_success() override;
        std::string get_shader_str() override;
        ~vertex_shader() override;

    };

    class fragment_shader final: public shader {
    public:
        explicit fragment_shader(const std::string& file_path);
        void compile() override;
        bool check_compile_success() override;
        std::string get_shader_str() override;
        ~fragment_shader() override;
    };

} // shader

