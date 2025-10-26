#pragma once
#include "ServiceLocator.hpp"
#include "Camera.hpp"

namespace core {

/// <summary>
/// Manage processing the scene for rendering.
/// </summary>
class ISceneRenderingManager : public IService {
public:
    virtual ~ISceneRenderingManager() = default;

    /// <summary>
    /// Initialize the VAO for the skybox.
    /// </summary>
    virtual void init_skybox_geometry() = 0;

    /// <summary>
    /// Draw the skybox.
    /// </summary>
    virtual void render_skybox(const Camera& camera) const = 0;
};

}