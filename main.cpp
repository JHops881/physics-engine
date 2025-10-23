#include "PhysSimApplication.hpp"
#include "PhysicsSystem.hpp"
#include "Renderer3D.hpp"
#include "ResourceManager.hpp"
#include "SceneRenderingManager.hpp"
#include "SceneManager.hpp"
#include "ServiceLocator.hpp"

int main() {
    std::shared_ptr<core::ServiceLocator> locator = std::make_shared<core::ServiceLocator>();
    
    std::shared_ptr<core::PhysicsSystem>         physics_system         = std::make_shared<core::PhysicsSystem>(locator);
    std::shared_ptr<core::Renderer3D>            renderer_3d            = std::make_shared<core::Renderer3D>(locator);
    std::shared_ptr<core::ResourceManager>       resource_manager       = std::make_shared<core::ResourceManager>(locator);
    std::shared_ptr<core::SceneManager>          scene_manager          = std::make_shared<core::SceneManager>(locator);
    std::shared_ptr<core::SceneRenderingManager> scene_rendering_manger = std::make_shared<core::SceneRenderingManager>(locator);

    locator->register_service(physics_system);
    locator->register_service(renderer_3d);
    locator->register_service(resource_manager);
    locator->register_service(scene_manager);
    locator->register_service(scene_rendering_manger);

    PhysSimApplication app(locator);

    try {
        app.run();
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}