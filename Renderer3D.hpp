#pragma once
#include "SparseSet.hpp"
#include "PhysicsSystem.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"
#include "Model.hpp"
#include "Material.hpp"
#include "IRenderer3D.hpp"
#include "ServiceLocator.hpp"

#include <variant>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <map>

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
class Renderer3D : public Service<IRenderer3D> {
private:
    std::shared_ptr<ServiceLocator> locator;
    std::map<CameraID, Camera> cameras;
    std::map<MeshID, Mesh> meshes;
    std::map<ModelID, Model> models;
    std::map<MaterialID, Material> materials;
    std::map<TextureID, Texture> textures;
public:
    /// <summary>
    /// Create a new Low Level Renderer.
    /// </summary>
    /// <param name="locator"></param>
    Renderer3D(std::shared_ptr<ServiceLocator> locator);

    void draw_geometry(
        const MeshID&     mesh_id,
        const glm::vec3&  position,
        const MaterialID& material_id,
        const CameraID&   camera_id)
    override;
};

}