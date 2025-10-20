#include "PhysSimApplication.hpp"
#include "MeshRegistry.hpp"
#include "PhysicsSystem.hpp"
#include "LowLevelRenderer.hpp"
#include "ShaderSystem.hpp"
#include "RenderingEngine.hpp"
#include "World.hpp"

int main() {
    std::shared_ptr<core::ShaderSystem>     shader_system    = std::make_shared<core::ShaderSystem>();
    std::shared_ptr<core::MeshRegistry>     mesh_registry    = std::make_shared<core::MeshRegistry>();
    std::shared_ptr<core::PhysicsSystem>    physics_system   = std::make_shared<core::PhysicsSystem>();
    std::shared_ptr<core::Renderer> low_lvl_renderer = std::make_shared<core::Renderer>(mesh_registry, physics_system);
    std::shared_ptr<core::ModelRegistry>    model_registry   = std::make_shared<core::ModelRegistry>();
    std::shared_ptr<core::World>            world            = std::make_shared<core::World>();
    std::shared_ptr<core::RenderingEngine>  rendering_engine = std::make_shared<core::RenderingEngine>(
        low_lvl_renderer, world, model_registry, mesh_registry, physics_system);
  
    PhysSimApplication app(shader_system, mesh_registry, physics_system, low_lvl_renderer, model_registry, world, rendering_engine);
    try { app.run(); }
    catch (const std::exception& exception) {
        std::cerr << exception.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}