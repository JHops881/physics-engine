#pragma once
#include "World.hpp"
#include "ServiceLocator.hpp"
#include "IRenderingEngine.hpp"
#include "IRenderer.hpp"
#include "Model.hpp"
#include "Mesh.hpp"
#include "Material.hpp"

#include <memory>

namespace core {

class RenderingEngine : Service<IRenderingEngine> {
private:
    std::shared_ptr<ServiceLocator> locator;
public:
    RenderingEngine(std::shared_ptr<ServiceLocator> locator);

    /// <summary>
    /// Render the world from a point of view.
    /// </summary>
    void render() const override;
};

}
