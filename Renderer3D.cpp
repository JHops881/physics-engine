#include "Renderer3D.hpp"

GLuint core::Renderer3D::compile_shader(const char* shader_str, int shader_type) {
    GLuint shader_id = glCreateShader(shader_type);
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

core::Renderer3D::Renderer3D(std::shared_ptr<ServiceLocator> locator) : locator(std::move(locator)) {}

GLuint core::Renderer3D::new_VBO(const std::vector<GLfloat>& vertex_data) const {
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(GLfloat), vertex_data.data(), GL_STATIC_DRAW);
    return vbo;
}

GLuint core::Renderer3D::new_EBO(const std::vector<GLushort>& indices) const {
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);
    return ebo;
}

GLuint core::Renderer3D::new_VAO(GLuint VBO, GLuint EBO, GLsizei attr_count, std::vector<GLint> sizes) const {
#ifdef _DEBUG
    if (attr_count != sizes.size()) {
        throw std::runtime_error("core::Renderer3D::new_VAO() failed. attr_count must == sizes.size()");
    }
#endif
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // Get stride
    int stride = 0;
    for (auto size : sizes) { stride += size; }

    int attribute_number = 0;
    GLint starting_point = 0;
    for (auto size : sizes) {
        glVertexAttribPointer(
            attribute_number, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(starting_point * sizeof(float)));
        glEnableVertexAttribArray(attribute_number);
        starting_point += size;
        attribute_number++;
    }
    return vao;
}

GLuint core::Renderer3D::new_shader_program(const char* vertex_shader_filepath, const char* fragment_shader_filepath) {
    std::string  vertex_shader_str   = utils::read_file(vertex_shader_filepath);
    std::string  fragment_shader_str = utils::read_file(fragment_shader_filepath);
    GLuint     vertex_shader   = compile_shader(vertex_shader_str.c_str(), GL_VERTEX_SHADER);
    GLuint     fragment_shader = compile_shader(fragment_shader_str.c_str(), GL_FRAGMENT_SHADER);
    GLuint     shader_program  = glCreateProgram();

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

void core::Renderer3D::delete_VBO(GLuint VBO) const {
    utils::time_and_name_log(__FUNCTION__);
    throw std::runtime_error("Unimplemented!");
}

void core::Renderer3D::delete_EBO(GLuint EBO) const {
    utils::time_and_name_log(__FUNCTION__);
    throw std::runtime_error("Unimplemented!");
}

void core::Renderer3D::delete_VAO(GLuint VAO) const {
    utils::time_and_name_log(__FUNCTION__);
    throw std::runtime_error("Unimplemented!");
}

void core::Renderer3D::delete_shader_program(GLuint shader) const {
    utils::time_and_name_log(__FUNCTION__);
    throw std::runtime_error("Unimplemented!");
}

void core::Renderer3D::draw_indexed_geometry(
    GLuint VAO, GLuint shader, GLuint texture, 
    GLsizei index_count, glm::vec3 position, const Camera& camera) const
{
#ifdef _DEBUG
    utils::time_and_name_log(__FUNCTION__);
    utils::print_vec3(position, "position");
    std::cout << std::format("shader_program_id={}", shader) << std::endl;
#endif
    glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), position);
    uint32_t  model_location = glGetUniformLocation(shader, "model");
    glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model_matrix));

    glm::mat4 view_matrix = camera.get_look_at();
    uint32_t  view_location = glGetUniformLocation(shader, "view");
    glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view_matrix));

    glm::mat4 projection_matrix = camera.get_perspective();
    uint32_t  projection_location = glGetUniformLocation(shader, "projection");
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection_matrix));

    glUseProgram(shader);
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);
}

