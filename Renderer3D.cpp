#include "Renderer3D.hpp"

GLuint core::Renderer3D::compile_shader(const char* shader_str, int shader_type) {
    // This is where the we init the opengl object and actual shader is compiled.
    GLuint shader_id = glCreateShader(shader_type);
    glShaderSource(shader_id, 1, &shader_str, NULL);
    glCompileShader(shader_id);
    // Just some error printout if something goes wrong.
    int success;
    char info_log[512];
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    // Just checking the shader type here for a more helpful error printout.
    const char* shader_type_str = "undefined"; // Initially nothing.
    if (shader_type == GL_VERTEX_SHADER) {
        shader_type_str = "vertex"; // this string is used below in the error printout.
    }
    else if (shader_type == GL_FRAGMENT_SHADER) {
        shader_type_str = "fragment";
    }
    if (!success) {
        glGetShaderInfoLog(shader_id, 512, NULL, info_log);
        std::string error = std::format("Error, {} shader failed to compile\n{}", shader_type_str, info_log);
        throw std::runtime_error(error);
    }
    return shader_id;
}

core::Renderer3D::Renderer3D(std::shared_ptr<ServiceLocator> locator)
    : locator(std::move(locator))
{
}

GLuint core::Renderer3D::new_VBO(const std::vector<GLfloat>& vertex_data) const {
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(GLfloat), vertex_data.data(), GL_STATIC_DRAW);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    return vbo;
}

GLuint core::Renderer3D::new_EBO(const std::vector<GLushort>& indices) const {
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    return ebo;
}

GLuint core::Renderer3D::new_VAO(GLuint VBO, std::vector<GLint> sizes, GLuint EBO) const {
    // prelaoding the opengl object and bindings
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    if (EBO != 0) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    }

    // Get stride
    int stride = 0;
    for (auto size : sizes) { stride += size; }

    int attribute_number = 0; // keeps track of which attribute ptr we are setting.
    GLint starting_point = 0; // The "statring point" for and attribute to start being read.
    for (auto size : sizes) {
        glVertexAttribPointer(
            attribute_number, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(starting_point * sizeof(float)));
        glEnableVertexAttribArray(attribute_number);
        starting_point += size; // The next attribute's starting point is at the end of this one's no?
        attribute_number++;
    }
    glBindVertexArray(0);
    if (EBO != 0) {
        glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);
    return vao;
}

GLuint core::Renderer3D::new_shader_program(const char* vertex_shader_filepath, const char* fragment_shader_filepath) {
    // Read in my shader sources and compile them.
    std::string vertex_shader_str = util::read_file(vertex_shader_filepath);
    GLuint      vertex_shader     = compile_shader(vertex_shader_str.c_str(), GL_VERTEX_SHADER);
    std::string fragment_shader_str = util::read_file(fragment_shader_filepath);
    GLuint      fragment_shader     = compile_shader(fragment_shader_str.c_str(), GL_FRAGMENT_SHADER);
    // Here's where the magic happens.
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    // Some extra error print out if the linking goes wrong.
    int success;
    char info_log[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        std::string error = std::format("Shader program failed to link.\n{}", info_log);
        throw std::runtime_error(error);
    }
    // cleanup
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    return shader_program;
}

void core::Renderer3D::delete_VBO(GLuint VBO) const {
    util::time_and_name_log(__FUNCTION__);
    throw std::runtime_error("Unimplemented!");
}

void core::Renderer3D::delete_EBO(GLuint EBO) const {
    util::time_and_name_log(__FUNCTION__);
    throw std::runtime_error("Unimplemented!");
}

void core::Renderer3D::delete_VAO(GLuint VAO) const {
    util::time_and_name_log(__FUNCTION__);
    throw std::runtime_error("Unimplemented!");
}

void core::Renderer3D::delete_shader_program(GLuint shader) const {
    util::time_and_name_log(__FUNCTION__);
    throw std::runtime_error("Unimplemented!");
}

void core::Renderer3D::draw_geometry(
    GLuint        VAO,
    GLuint        shader,
    glm::vec3     position,
    const Camera& camera,
    bool          indexed,
    GLsizei       count,
    int           gl_texture_type,
    GLuint        texture) 
const {
    // This needs to be called before changing the transformation matrices
    glUseProgram(shader);
    // Model - Based on where the mesh is being drawn in the 3d space.
    glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), position);
    uint32_t  model_location = glGetUniformLocation(shader, "model");
    glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model_matrix));
    // View - Based on 'from what angle/postion' the mesh will be perceived in the 3d space.
    glm::mat4 view_matrix = camera.get_look_at();
    uint32_t  view_location = glGetUniformLocation(shader, "view");
    glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view_matrix));
    // Projection - Based on the perspective settings of the camera/application.
    glm::mat4 projection_matrix = camera.get_perspective();
    uint32_t  projection_location = glGetUniformLocation(shader, "projection");
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection_matrix));

    glBindVertexArray(VAO);
    if (gl_texture_type != 0) {
        glBindTexture(gl_texture_type, texture);
    }
    if (indexed == true) {
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, 0);
    }
    else {
        glDrawArrays(GL_TRIANGLES, 0, count);
    }
    glBindVertexArray(0);
}

void core::Renderer3D::draw_illuminator(
    GLuint        VAO,
    GLuint        shader,
    glm::vec3     position,
    const Camera& camera,
    GLsizei       count)
const {
    // This needs to be called before changing the transformation matrices
    glUseProgram(shader);
    // Model - Based on where the mesh is being drawn in the 3d space.
    glm::mat4 model_matrix = glm::scale(glm::translate(glm::mat4(1.0f), position), glm::vec3(0.2f)); // shrink
    uint32_t  model_location = glGetUniformLocation(shader, "model");
    glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model_matrix));
    // View - Based on 'from what angle/postion' the mesh will be perceived in the 3d space.
    glm::mat4 view_matrix = camera.get_look_at();
    uint32_t  view_location = glGetUniformLocation(shader, "view");
    glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view_matrix));
    // Projection - Based on the perspective settings of the camera/application.
    glm::mat4 projection_matrix = camera.get_perspective();
    uint32_t  projection_location = glGetUniformLocation(shader, "projection");
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection_matrix));

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, count);
    glBindVertexArray(0);
}

void core::Renderer3D::draw_illuminated(
    GLuint        VAO,
    GLuint        shader,
    glm::vec3     position,
    const Camera& camera,
    GLsizei       count,
    Material      material,
    glm::vec3     light_color,
    glm::vec3     light_position)
const {
    // This needs to be called before changing the transformation matrices
    glUseProgram(shader);

    // Affected by Lights
    util::set_uniform_vec3(shader, "light.position", light_position);
    // light color.
    util::set_uniform_vec3(shader, "light.ambient", glm::vec3(0.2f)); // hardcoded
    util::set_uniform_vec3(shader, "light.diffuse", glm::vec3(0.5f));
    util::set_uniform_vec3(shader, "light.specular", glm::vec3(1.0f));
    const GLfloat MAX_SHININESS = 128.0f; // magic number :)

    // Material
    util::set_uniform_vec3(shader, "material.specular", material.specular);
    util::set_uniform_float(shader, "material.shininess", material.shininess * MAX_SHININESS);

    // Model - Based on where the mesh is being drawn in the 3d space.
    glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), position);
    util::set_uniform_mat4(shader, "model", model_matrix);

    // Calculate the normal matrix - essential for non-uniformly scaled meshes.
    // The normal matrix properly transforms normals to world coordinates.
    glm::mat3 normal_matrix = glm::transpose(glm::inverse(model_matrix));
    util::set_uniform_mat3(shader, "normalMatrix", normal_matrix);

    // Hand off the position of the viewer - for specular lighting.
    util::set_uniform_vec3(shader, "viewPos", camera.get_position());

    // View - Based on 'from what angle/postion' the mesh will be perceived in the 3d space.
    glm::mat4 view_matrix = camera.get_look_at();
    util::set_uniform_mat4(shader, "view", view_matrix);

    // Projection - Based on the perspective settings of the camera/application.
    glm::mat4 projection_matrix = camera.get_perspective();
    util::set_uniform_mat4(shader, "projection", projection_matrix);

    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, material.texture);
    glDrawArrays(GL_TRIANGLES, 0, count);
    glBindVertexArray(0);
}


