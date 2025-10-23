#pragma once
#include "ServiceLocator.hpp"
#include "Mesh.hpp"
#include "Model.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace core {

/// <summary>
/// A service that manages loading assets/resources (sounds, models, textures, etc.) in and out of memory.
/// </summary>
class IResourceManager : public IService {
public:
    virtual ~IResourceManager() = default;

    /// <summary>
    /// Load a texture into memory.
    /// </summary>
    /// <param name="texture_filepath">: The path for the texture image file.</param>
    /// <returns>An unsigned int that is the ID of the texture.</returns>
    virtual GLuint load_texture(std::string texture_filepath) const = 0;
    
    /// <summary>
    /// Load a cube map into memory.
    /// </summary>
    /// <param name="texture_filepaths">: A list of file paths to the textures of the cube sides.
    /// This must be in the order of RLTBFB.</param>
    /// <returns>An unsigned int taht is the ID of the cube map.</returns>
    virtual GLuint load_cubemap(std::vector<std::string> texture_filepaths) const = 0;

    /// <summary>
    /// Load a mesh into memory.
    /// </summary>
    /// <param name="vertex_data">: The vertex data that defines the 3D geometry and attributes.</param>
    /// <param name="indices">: A collection of ordered indices that specify the (primitive) surfaces of the mesh.</param>
    /// <param name="texture">: The texture to be associated with the 3D geometry.</param>
    /// <returns>: An ID that refers to the mesh.</returns>
    virtual MeshID load_mesh(const std::vector<GLfloat> vertex_data, const std::vector<GLshort> indices, GLuint texture) = 0;

    /// <summary>
    /// Create a new model from a collection of meshes.
    /// </summary>
    /// <param name="meshes">: The collection of meshes that make up the model.</param>
    /// <returns>An Id that refers to the model.</returns>
    virtual ModelID new_model(const std::vector<MeshID> meshes) = 0;

    /// <summary>
    /// Get a mesh.
    /// </summary>
    /// <param name="mesh_id">: The ID that refers to the mesh that you want.</param>
/// /// <returns>A reference to the mesh.</returns>
    virtual Mesh& get_mesh(MeshID mesh_id) = 0;

    /// <summary>
    /// Get a model.
    /// </summary>
    /// <param name="model_id">: The ID that refers to the model that you want.</param>
/// /// <returns>A reference to the model.</returns>
    virtual Model& get_model(ModelID model_id) = 0;

    /// <summary>
    /// Unload a texture from memory.
    /// </summary>
    /// <param name="texture">: The iD of the texture that you want to unload.</param>
    virtual void unload_texture(GLuint texture) const = 0;

    /// <summary>
    /// Unload a cubemap from memory.
    /// </summary>
    /// <param name="cubemap">: The ID of the cubemap that you want to unload.</param>
    virtual void unload_cubemap(GLuint cubemap) const = 0;

    /// <summary>
    /// Unload a mesh from memory.
    /// </summary>
    /// <param name="mesh_id">: The ID of the mesh that you want to unload.</param>
    virtual void unload_mesh(MeshID mesh_id) = 0;

    /// <summary>
    /// Delete a mesh.
    /// </summary>
    /// <param name="model_id">: The ID of the mesh that you want to delete.</param>
    virtual void delete_model(ModelID model_id) = 0;
};

}