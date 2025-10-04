#pragma once
#include "MeshRegistry.hpp"
#include "SparseSet.hpp"
#include "PhysicsSystem.hpp"

#include <variant>
#include <memory>
#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

namespace gfx
{

struct RenderableID
{
    uint32_t value;
    // Allows use like a uint32_t
    operator uint32_t() const
    {
         return value;
    }
};

struct Renderable
{
    MeshID                                        mesh_id;
    std::variant<phys::StaticID, phys::DynamicID> physics_id;
};

class RenderingSystem
{
  private:
    std::shared_ptr<MeshRegistry>        mesh_registry;
    std::shared_ptr<phys::PhysicsSystem> physics_system;
    SparseSet<Renderable>                renderables;

    glm::mat4 model_matrix      = glm::mat4(1.0f);
    glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);

  public:
    RenderingSystem(std::shared_ptr<MeshRegistry> mesh_registry, std::shared_ptr<phys::PhysicsSystem> physics_system);

    RenderableID new_renderable(const Renderable& renderable);

    void remove_renderable(RenderableID id);
    
    Renderable& get_renderable(RenderableID id);

    void render();

};

}