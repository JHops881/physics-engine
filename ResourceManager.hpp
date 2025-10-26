#pragma once
#include "IResourceManager.hpp"
#include "ServiceLocator.hpp"
#include "stb_image.h"
#include "Mesh.hpp"
#include "Model.hpp"
#include "utils.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <memory>
#include <stdexcept>

namespace core {

class ResourceManager : public Service<IResourceManager> {
private:
    std::shared_ptr<ServiceLocator> locator;
public:
    ResourceManager(std::shared_ptr<ServiceLocator> locator);

    virtual GLuint load_texture(std::string texture_filepath) const override;

    virtual GLuint load_cubemap(std::vector<std::string> texture_filepaths) const override;

    virtual MeshID load_mesh(const std::vector<GLfloat> vertex_data, const std::vector<GLshort> indices, GLuint texture) override;

    virtual ModelID new_model(const std::vector<MeshID> meshes) override;

    virtual Mesh& get_mesh(MeshID mesh_id) override;

    virtual Model& get_model(ModelID model_id) override;

    virtual void unload_texture(GLuint texture) const override;

    virtual void unload_cubemap(GLuint cubemap) const override;

    virtual void unload_mesh(MeshID mesh_id) override;

    virtual void delete_model(ModelID model_id) override;
};

}