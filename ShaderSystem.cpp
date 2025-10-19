#include "ShaderSystem.hpp"

std::string core::read_file(const char* filepath) {
    std::ifstream file(filepath);
    std::string file_contents{ std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
    return file_contents;
}

uint32_t core::ShaderSystem::compile_shader(const char* shader_str, int shader_type) {
    uint32_t shader_id = glCreateShader(shader_type);
    glShaderSource(shader_id, 1, &shader_str, NULL);
    glCompileShader(shader_id);

    int success;
    char info_log[512];
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

    const char* shader_type_str = "undefined";
    if (shader_type == GL_VERTEX_SHADER) { shader_type_str = "vertex"; }
    else if (shader_type == GL_FRAGMENT_SHADER) { shader_type_str = "fragment"; }
    if (!success) {
        glGetShaderInfoLog(shader_id, 512, NULL, info_log);
        std::string error = std::format("Error, {} shader failed to compile\n{}", shader_type_str, info_log);
        throw std::runtime_error(error);
    }
    return shader_id;
}

uint32_t core::ShaderSystem::create_shader_program(const char* vertex_shader_source, const char* fragment_shader_source) {
    std::string  vertex_shader_str   = read_file(vertex_shader_source);
    std::string  fragment_shader_str = read_file(fragment_shader_source);
    uint32_t     vertex_shader       = compile_shader(vertex_shader_str.c_str(), GL_VERTEX_SHADER);
    uint32_t     fragment_shader     = compile_shader(fragment_shader_str.c_str(), GL_FRAGMENT_SHADER);
    uint32_t     shader_program      = glCreateProgram();

    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    int success;
    char info_log[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        std::string error = std::format("Shader program failed to link.\n{}", info_log);
        throw std::runtime_error(error);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
}
