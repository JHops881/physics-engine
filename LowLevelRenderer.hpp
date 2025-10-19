#pragma once
#include "MeshRegistry.hpp"
#include "SparseSet.hpp"
#include "PhysicsSystem.hpp"
#include "ModelRegistry.hpp"
#include "Camera.hpp"

#include <variant>
#include <memory>
#include <stdexcept>
#include <unordered_map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

namespace core {

/// <summary>
/// Encompasses all of the raw rendering facilities of the rendering engine. The focus is to render geometric primitives that
/// are submitted to it both as quickly and as richly as possible. The low level renderer does not have an interest in what
/// sections of the scene are visible and which are not. 
/// Components: Materials & Shaders, Static and Dynamic Lighting, Cameras, Text and Fonts, Primitive Submission, 
/// Viewports & Virutal Screens, Textures and Surface Management, Debug Drawing, Graphics Device Interface.
/// "Game Engine Architecture" by Jason Gregory, pg 59 or part II
/// </summary>
class LowLevelRenderer {
private:
    std::shared_ptr<MeshRegistry>  mesh_registry;
    std::shared_ptr<PhysicsSystem> physics_system;
public:
    std::unordered_map<CameraID, Camera> cameras;
    
    /// <summary>
    /// Create a new Low Level Renderer.
    /// </summary>
    /// <param name="mesh_registry">: Mesh Registry</param>
    /// <param name="physics_system">: Physics System</param>
    LowLevelRenderer(std::shared_ptr<MeshRegistry> mesh_registry, std::shared_ptr<PhysicsSystem> physics_system);

    /// <summary>
    /// Draw a primitives to the screen from a view point.
    /// </summary>
    /// <param name="mesh"></param>
    /// <param name="position"></param>
    /// <param name="material"></param>
    /// <param name="camera"></param>
    void draw_geometry(const Mesh& mesh, const glm::vec3& position, const Material& material, const Camera& camera);
};

}