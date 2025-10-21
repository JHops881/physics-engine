#include "PhysSimApplication.hpp"
#include "PhysicsSystem.hpp"
#include "Renderer3D.hpp"
#include "ServiceLocator.hpp"

int main() {
    std::shared_ptr<core::ServiceLocator> locator = std::make_shared<core::ServiceLocator>();
    
    std::shared_ptr<core::PhysicsSystem> physics_system = std::make_shared<core::PhysicsSystem>(locator);
    std::shared_ptr<core::Renderer3D>    renderer_3d    = std::make_shared<core::Renderer3D>(locator);

    locator->register_service(physics_system);
    locator->register_service(renderer_3d);

    PhysSimApplication app(locator);
    try { app.run(); }
    catch (const std::exception& exception) {
        std::cerr << exception.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}