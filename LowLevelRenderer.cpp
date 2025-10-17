#include "LowLevelRenderer.hpp"

core::LowLevelRenderer::LowLevelRenderer
(
    std::shared_ptr<MeshRegistry>        mesh_registry,
    std::shared_ptr<core::PhysicsSystem> physics_system
)
    :
    mesh_registry(std::move(mesh_registry)),
    physics_system(std::move(physics_system))
{
}

core::RenderableID core::LowLevelRenderer::new_renderable(const Renderable& renderable)
{
    RenderableID id = RenderableID(renderables.add(renderable));
    return id;
}

void core::LowLevelRenderer::remove_renderable(RenderableID id)
{
#ifdef _DEBUG
    if (!renderables.has(id))
    {
        throw std::runtime_error("gfx::RenderingSystem::remove_renderable() failed. No renderable with that ID exists.");
    }
#endif

    renderables.remove(id);
}

core::Renderable& core::LowLevelRenderer::get_renderable(RenderableID id)
{
    if (renderables.has(id))
    {
        return renderables.get(id);
    }
    else
    {
        throw std::runtime_error("gfx::RenderingSystem::get_renderable() failed. No renderable with that ID exists.");
    }
}

void core::LowLevelRenderer::render()
{
    for (Renderable& renderable : renderables.get_dense())
    {
        glm::mat4 view_matrix = glm::mat4(1.0f);

        glm::vec3& position = physics_system->get_point_mass(renderable.physics_id).position;
        view_matrix = glm::translate(view_matrix, position);

        Mesh& mesh = mesh_registry->get_mesh(renderable.mesh_id);

        uint32_t model_location = glGetUniformLocation(mesh.shader, "model");
        glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model_matrix));

        uint32_t view_location = glGetUniformLocation(mesh.shader, "view");
        glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view_matrix));

        uint32_t projection_location = glGetUniformLocation(mesh.shader, "projection");
        glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection_matrix));

        glUseProgram(mesh.shader);
        glBindVertexArray(mesh.vao);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}
