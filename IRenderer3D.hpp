#pragma once
#include "ServiceLocator.hpp"
#include "Camera.hpp"

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

/*
TODO: GPU Instancing: Be able to draw a ton of the same object all over the scene with a single draw call
TODO: GPU driven, indirect rendering
[Scene Manager Service] -> [3D Rendering Service] -> [OpenGL] -> [Compute Shaders (LOD, Culling)] -> [GPU]

So you'll upload all the geometry, all the instances, etc. to the GPU
Then, you dispatch compute commands that can determine what is visible
From the visible set, you can populate a buffer containing draw commands
Then, you would issue an indirect draw call, using the buffer containing the draw commands

A better metric to use for LOD selection is determining the % of the screen size the object takes (vertical, horizontal,
area, one of these metrics), then using that to select the best suited LOD

Also, another advanced LOD selection technique would be to determine the triangle sizes of the object in pixels,
and if the triangles are really small, swap out to a lower LOD (less detail)
*/

namespace core {

/// <summary>
/// An abstraction on top of the graphics API--OpenGL--and provides a less complex interface.
/// </summary>
class IRenderer3D : public IService {
private:
    /// <summary>
    /// Compile a shader.
    /// </summary>
    /// <param name="shader_str">: The shader source code as a c-style string.</param>
    /// <param name="shader_type">: An enum of what kind of shader is being compiled.</param>
    /// <returns>An integer that is an ID that references the shader OpenGL object that was created from the compilation.</returns>
    virtual GLuint compile_shader(const char* shader_str, int shader_type) = 0;
public:
    virtual ~IRenderer3D() = default;

    /// <summary>
    /// Create a new Vertex Buffer Object (VBO).
    /// </summary>
    /// <param name="vertex_data">: A collection of all the vertex data that you want to store in the buffer.</param>
    /// <returns>An unsigned integer that is an ID that refers to the (VBO) OpenGL object created.</returns>
    virtual GLuint new_VBO(const std::vector<GLfloat>& vertex_data) const = 0;

    /// <summary>
    /// Create a new Element Buffer Object (EBO).
    /// </summary>
    /// <param name="indices">: A collection of the indices that you want to store in the buffer.</param>
    /// <returns>An unsigned integer that is an ID that refers to the (EBO) OpenGL object created.</returns>
    virtual GLuint new_EBO(const std::vector<GLushort>& indices) const = 0;

    /// <summary>
    /// Create a new Vertex Array Object (VAO).
    /// </summary>
    /// <param name="VBO">: The VBO that the VAO will store the configuration for.</param>
    /// <param name="EBO">: The EBO associated with the VBO.</param>
    /// <param name="attr_count">: the total count of VAO attributes that you want.</param>
    /// <param name="sizes">: A series of int values that represent the length of each attribute in the respective order.</param>
    /// <returns>An unsigned integer that is an ID that refers to the (VAO) OpenGL object created.</returns>
    virtual GLuint new_VAO(GLuint VBO, GLuint EBO, GLsizei attr_count, GLint sizes[]) const = 0;

    /// <summary>
    /// Create a new shader program.
    /// </summary>
    /// <param name="vertex_shader_filepath">: The path to the vertex shader file.</param>
    /// <param name="fragment_shader_filepath">: The path to the fragment shader file.</param>
    /// <returns>An insigned integer that is an ID that refers to the shader program OpenGL object created.</returns>
    virtual GLuint new_shader_program(const char* vertex_shader_filepath, const char* fragment_shader_filepath) const = 0;

    /// <summary>
    /// Delete a Vertex Buffer Object (VBO).
    /// </summary>
    /// <param name="VBO">The ID of the VBO you want to delete.</param>
    virtual void delete_VBO(GLuint VBO) const = 0;

    /// <summary>
    /// Delete and Element Buffer Object (EBO).
    /// </summary>
    /// <param name="EBO">The ID of the EBO you want to delete.</param>
    virtual void delete_EBO(GLuint EBO) const = 0;

    /// <summary>
    /// Delete a Vertex Array Object.
    /// </summary>
    /// <param name="VAO">The ID of the VAO you want to delete.</param>
    virtual void delete_VAO(GLuint VAO) const = 0;

    /// <summary>
    /// Delete a shader program.
    /// </summary>
    /// <param name="shader">The ID of the shader program that you want to delete.</param>
    virtual void delete_shader_program(GLuint shader) const = 0;

    /// <summary>
    /// Draw/Render geometry that is associated with an Element Buffer Object.
    /// </summary>
    /// <param name="VAO">: The Vertex Array Object of the configuration and mesh(es) you want to draw.</param>
    /// <param name="shader">: The shader program that the mesh will be rendered using.</param>
    /// <param name="texture">: The texture that you would like to apply to the mesh.</param>
    /// <param name="index_count">: The numbder of indices that you want to draw. (also called elements)</param>
    /// <param name="position">: The location in the scene where the mesh will be drawn.</param>
    /// <param name="camera_id">: A camera--which defines the point of view that the mesh will be rendered from.</param>
    virtual void draw_indexed_geometry(
        GLuint VAO,
        GLuint shader,
        GLuint texture,
        GLsizei index_count,
        glm::vec3 position,
        const CameraID& camera_id) const = 0;
};

}