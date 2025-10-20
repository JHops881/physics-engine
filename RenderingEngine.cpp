#include "RenderingEngine.hpp"

core::RenderingEngine::RenderingEngine(std::shared_ptr<ServiceLocator> locator)
{
}

void core::RenderingEngine::render() const {
    auto world = locator->get_service<IWorld>();
#ifdef _DEBUG
    utils::time_and_name_log(__FUNCTION__);
    for (auto& object : world->get_objects()) {
        std::cout << "Rendering an Object." << std::endl;
    }
#endif
    auto renderer = locator->get_service<IRenderer>();
    for (auto& object : world->get_objects()) {
        Model& model = renderer->models(object.model_id); // Pick up here. // need to hide the models behind an interface
        Mesh&  mesh  = mesh_registry->get_mesh(model.mesh_id);
        low_lvl_renderer->draw_geometry(mesh, physics_system->get_point_mass(object.pm_id).position, model.material, camera);
    }
}


