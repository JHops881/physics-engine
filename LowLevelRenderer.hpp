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

namespace core
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
    MeshID           mesh_id;
    core::ParticleID physics_id;
};

/// <summary>
/// Encompasses all of the raw rendering facilities of the rendering engine. The focus is to render geometric primitives that
/// are submitted to it both as quickly and as richly as possible. The low level renderer does not have an interest in what
/// sections of the scene are visible and which are not. 
/// Components: Materials & Shaders, Static and Dynamic Lighting, Cameras, Text and Fonts, Primitive Submission, 
/// Viewports & Virutal Screens, Textures and Surface Management, Debug Drawing, Graphics Device Interface.
/// "Game Engine Architecture" by Jason Gregory, pg 59 or part II
/// </summary>
class LowLevelRenderer
{
  private:
    std::shared_ptr<MeshRegistry>        mesh_registry;
    std::shared_ptr<core::PhysicsSystem> physics_system;
    SparseSet<Renderable>                renderables;

    float near_clip_plane = 0.1f;
    float far_clip_plane  = 100.0f;
    float aspect_ratio    = 16.0f / 9.0f;
    float fov             = glm::radians(72.0f); // always in radians

    glm::mat4 model_matrix      = glm::mat4(1.0f);
    glm::mat4 projection_matrix = glm::perspective(fov, aspect_ratio, near_clip_plane, far_clip_plane);

  public:
    LowLevelRenderer(std::shared_ptr<MeshRegistry> mesh_registry, std::shared_ptr<core::PhysicsSystem> physics_system);

    RenderableID new_renderable(const Renderable& renderable);

    void remove_renderable(RenderableID id);
    
    Renderable& get_renderable(RenderableID id);

    void render();

};

}