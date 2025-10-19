#pragma once
#include "LowLevelRenderer.hpp"
#include "World.hpp"

#include <memory>

namespace core {

class RenderingEngine {
private:
    std::shared_ptr<LowLevelRenderer>    low_lvl_renderer;
    std::shared_ptr<World>               world;
    std::shared_ptr<ModelRegistry>       model_registry;
    std::shared_ptr<MeshRegistry>        mesh_registry;
    std::shared_ptr<core::PhysicsSystem> physics_system; 
public:
    RenderingEngine(
        std::shared_ptr<LowLevelRenderer>    low_lvl_renderer,
        std::shared_ptr<World>               world,
        std::shared_ptr<ModelRegistry>       model_registry,
        std::shared_ptr<MeshRegistry>        mesh_registry,
        std::shared_ptr<core::PhysicsSystem> physics_system);

    /// <summary>
    /// Render the world from a point of view.
    /// </summary>
    void render(const Camera& camera) const;
};

}
