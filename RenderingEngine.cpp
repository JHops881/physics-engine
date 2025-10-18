#include "RenderingEngine.hpp"

core::RenderingEngine::RenderingEngine
(
    std::shared_ptr<LowLevelRenderer>    low_lvl_renderer,
    std::shared_ptr<World>               world,
    std::shared_ptr<ModelRegistry>       model_registry,
    std::shared_ptr<MeshRegistry>        mesh_registry,
    std::shared_ptr<core::PhysicsSystem> physics_system
)
    :
    low_lvl_renderer(std::move(low_lvl_renderer)),
    world(std::move(world)),
    model_registry(std::move(model_registry)),
    mesh_registry(std::move(mesh_registry)),
    physics_system(std::move(physics_system))
{
}

void core::RenderingEngine::render(const Camera& camera) const
{
#ifdef _DEBUG
    utils::time_and_name_log(__FUNCTION__);

    for (auto& object : world->get_objects())
    {
        std::cout << "Rendering an Object." << std::endl;
    }
#endif
    for (auto& object : world->get_objects())
    {
        Model& model = model_registry->get_model(object.model_id);
        Mesh& mesh   = mesh_registry->get_mesh(model.mesh_id);
        
        low_lvl_renderer->draw_geometry(mesh, physics_system->get_point_mass(object.pm_id).position, model.material, camera);
    }
}
