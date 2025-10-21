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

    virtual GLuint new_VBO(const std::vector<GLfloat>& vertex_data) const override;

    virtual GLuint new_EBO(const std::vector<GLushort>& indices) const override;

    virtual GLuint new_VAO(GLuint VBO, GLuint EBO, GLsizei attr_count, std::vector<GLint> sizes) const override;

    virtual GLuint new_shader_program(const char* vertex_shader_filepath, const char* fragment_shader_filepath) override;

    virtual void delete_VBO(GLuint VBO) const override;

    virtual void delete_EBO(GLuint EBO) const override;

    virtual void delete_VAO(GLuint VAO) const override;

    virtual void delete_shader_program(GLuint shader) const override;

    virtual void draw_indexed_geometry(
        GLuint VAO,
        GLuint shader,
        GLuint texture,
        GLsizei index_count,
        glm::vec3 position,
        const Camera& camera) const override;
};

}