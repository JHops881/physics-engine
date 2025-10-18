#pragma once
#include "MeshRegistry.hpp"
#include "SparseSet.hpp"
#include "PhysicsSystem.hpp"
#include "ModelRegistry.hpp"

#include <variant>
#include <memory>
#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

namespace core
{

/// <summary>
/// A camera that is a point of view within the rendered world.
/// </summary>
struct Camera
{
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 reverse_direction;
    glm::vec3 up;
    glm::vec3 right;

    float near_clip_plane;
    float far_clip_plane;
    float aspect_ratio;
    float fov;
};

/// <summary>
/// An ID for a camera.
/// </summary>
struct CameraID
{
    uint32_t value;
    // Allows use like a uint32_t
    operator uint32_t() const
    {
        return value;
    }
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
    std::shared_ptr<MeshRegistry>  mesh_registry;
    std::shared_ptr<PhysicsSystem> physics_system;
    SparseSet<Camera>              cameras;   

  public:

    /// <summary>
    /// Create a new Low Level Renderer.
    /// </summary>
    /// <param name="mesh_registry">: Mesh Registry</param>
    /// <param name="physics_system">: Physics System</param>
    LowLevelRenderer(std::shared_ptr<MeshRegistry> mesh_registry, std::shared_ptr<PhysicsSystem> physics_system);

    /// <summary>
    /// 
    /// </summary>
    /// <param name="mesh"></param>
    /// <param name="position"></param>
    /// <param name="material"></param>
    /// <param name="camera"></param>
    void draw_geometry(const Mesh& mesh, const glm::vec3& position, const Material& material, const Camera& camera);

    /// <summary>
    /// Add a camera.
    /// </summary>
    /// <param name="position">: initial position of the camera in 3D world space</param>
    /// <param name="up">: The direction that the top of the camera is facing</param>
    /// <returns>The ID of the camera</returns>
    CameraID add_camera(const glm::vec3& position, const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));

    /// <summary>
    /// Get a camera.
    /// </summary>
    /// <param name="id">: the ID of the camera</param>
    /// <returns>A reference to the camera</returns>
    Camera& get_camera(const CameraID id);

    /// <summary>
    /// Remove a camera.
    /// </summary>
    /// <param name="id">: the ID of the camera that you want to remove</param>
    void remove_camera(const CameraID id);

};

}