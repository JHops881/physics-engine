#pragma once
#include "ServiceLocator.hpp"
#include "ISceneRenderingManager.hpp"
#include "ISceneManager.hpp"
#include "IRenderer3D.hpp"

#include <glad/glad.h>

namespace core {

/// <summary>
/// Manage processing the scene for rendering.
/// </summary>
class SceneRenderingManager : public Service<ISceneRenderingManager> {
private:
    GLuint skybox_vao;
    GLuint skybox_shader;
    std::shared_ptr<ServiceLocator> locator;
public:
    SceneRenderingManager(std::shared_ptr<ServiceLocator> locator);

    void init_skybox_geometry() override;

    void render_skybox(const Camera& camera) const override;
};
}