#pragma once
#include "SparseSet.hpp"
#include "PhysicsSystem.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"
#include "Model.hpp"
#include "Material.hpp"
#include "IRenderer3D.hpp"
#include "ServiceLocator.hpp"

#include <variant>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

namespace core {

class Renderer3D : public Service<IRenderer3D> {
private:
    std::shared_ptr<ServiceLocator> locator;
    std::map<CameraID, Camera> cameras;

    GLuint compile_shader(const char* shader_str, int shader_type) override;
public:
    /// <summary>
    /// Create a new Low Level Renderer.
    /// </summary>
    /// <param name="locator"></param>
    Renderer3D(std::shared_ptr<ServiceLocator> locator);

    GLuint new_VBO(const std::vector<GLfloat>& vertex_data) const override;

    GLuint new_EBO(const std::vector<GLushort>& indices) const override;

    GLuint new_VAO(GLuint VBO, std::vector<GLint> sizes, GLuint EBO = 0) const override;

    GLuint new_shader_program(const char* vertex_shader_filepath, const char* fragment_shader_filepath) override;

    void delete_VBO(GLuint VBO) const override;

    void delete_EBO(GLuint EBO) const override;

    void delete_VAO(GLuint VAO) const override;

    void delete_shader_program(GLuint shader) const override;

    void draw_geometry(
        GLuint VAO,
        GLuint shader,
        glm::vec3 position,
        const Camera& camera,
        bool indexed,
        GLsizei count,
        int gl_texture_type = 0,
        GLuint texture = 0) const override;

    void draw_illuminator(
        GLuint VAO,
        GLuint shader,
        glm::vec3 position,
        const Camera& camera,
        GLsizei count) const override;

    void draw_illuminated(
        GLuint VAO,
        GLuint shader,
        glm::vec3 position,
        const Camera& camera,
        GLsizei count,
        glm::vec3 object_color,
        glm::vec3 light_color,
        glm::vec3 light_position) const override;
};

}