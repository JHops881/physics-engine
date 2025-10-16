#include "PhysSimApplication.hpp"
#include "MeshRegistry.hpp"
#include "PhysicsSystem.hpp"
#include "RenderingSystem.hpp"
#include "ShaderSystem.hpp"

int main()
{
    std::shared_ptr<core::ShaderSystem>    shader_system    = std::make_shared<core::ShaderSystem>();
    std::shared_ptr<core::MeshRegistry>    mesh_registry    = std::make_shared<core::MeshRegistry>();
    std::shared_ptr<core::PhysicsSystem>  physics_system   = std::make_shared<core::PhysicsSystem>();
    std::shared_ptr<core::RenderingSystem> rendering_system = std::make_shared<core::RenderingSystem>(mesh_registry, physics_system);

    PhysSimApplication app(shader_system, mesh_registry, physics_system, rendering_system);
    try
    {
        app.run();
    }
    catch (const std::exception& exception)
    {
        std::cerr << exception.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}