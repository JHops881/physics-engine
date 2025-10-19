#pragma once

#include <cstdint>
#include <vector>
#include <format>
#include <iostream>
#include <fstream>

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace core {

std::string read_file(const char* filepath);

class ShaderSystem {
private:
    uint32_t compile_shader(const char* shader_str, int shader_type);
public:
    uint32_t create_shader_program(const char* vertex_shader_source, const char* fragment_shader_source);
};

}