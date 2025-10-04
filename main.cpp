#include "PhysSimApplication.hpp"
#include "MeshRegistry.hpp"
#include "PhysicsSystem.hpp"
#include "RenderingSystem.hpp"
#include "ShaderSystem.hpp"

int main()
{
    std::shared_ptr<gfx::ShaderSystem>    shader_system    = std::make_shared<gfx::ShaderSystem>();
    std::shared_ptr<gfx::MeshRegistry>    mesh_registry    = std::make_shared<gfx::MeshRegistry>();
    std::shared_ptr<phys::PhysicsSystem>  physics_system   = std::make_shared<phys::PhysicsSystem>();
    std::shared_ptr<gfx::RenderingSystem> rendering_system = std::make_shared<gfx::RenderingSystem>(mesh_registry, physics_system);

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