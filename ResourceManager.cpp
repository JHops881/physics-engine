#include "ResourceManager.hpp"

core::ResourceManager::ResourceManager(std::shared_ptr<ServiceLocator> locator) : locator(locator) {}

GLuint core::ResourceManager::load_texture(std::string texture_filepath) const {
    int width;
    int height;
    int nr_channels;
    unsigned char* data = stbi_load(texture_filepath.c_str(), &width, &height, &nr_channels, 0);
    if (!data) {
        throw std::runtime_error("core::ResourceManager::load_texture() failed. Couldn't load texture with stbi_load().");
    }
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    return texture;
}

GLuint core::ResourceManager::load_cubemap(std::vector<std::string> texture_filepaths) const {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    int width;
    int height;
    int nr_channels;

    for (size_t i = 0; i < texture_filepaths.size(); i++) {
        unsigned char* data = stbi_load(texture_filepaths[i].c_str(), &width, &height, &nr_channels, 0);
        if (!data) {
            throw std::runtime_error("core::ResourceManager::load_cubemap() failed.");
        }
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return texture;
}

core::MeshID core::ResourceManager::load_mesh(
    const std::vector<GLfloat> vertex_data,
    const std::vector<GLshort> indices,
    GLuint texture)
{
    return MeshID();
}

core::ModelID core::ResourceManager::new_model(const std::vector<MeshID> meshes) {
    utils::time_and_name_log(__FUNCTION__);
    throw std::runtime_error("Implement this!");
}

core::Mesh& core::ResourceManager::get_mesh(MeshID mesh_id) {
    utils::time_and_name_log(__FUNCTION__);
    throw std::runtime_error("Implement this!");
}

core::Model& core::ResourceManager::get_model(ModelID model_id) {
    utils::time_and_name_log(__FUNCTION__);
    throw std::runtime_error("Implement this!");
}

void core::ResourceManager::unload_texture(GLuint texture) const {
    utils::time_and_name_log(__FUNCTION__);
    throw std::runtime_error("Implement this!");
}

void core::ResourceManager::unload_cubemap(GLuint cubemap) const {
    utils::time_and_name_log(__FUNCTION__);
    throw std::runtime_error("Implement this!");
}

void core::ResourceManager::unload_mesh(MeshID mesh_id) {
    utils::time_and_name_log(__FUNCTION__);
    throw std::runtime_error("Implement this!");
}

void core::ResourceManager::delete_model(ModelID model_id) {
    utils::time_and_name_log(__FUNCTION__);
    throw std::runtime_error("Implement this!");
}

