#pragma once
#include "ServiceLocator.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "Camera.hpp"

#include <glm/glm.hpp>

namespace core {

class IRenderer : public IService {
public:
    virtual ~IRenderer() = default;

    virtual void draw_geometry(const Mesh& mesh, const glm::vec3& position, const Material& material, const Camera& camera) = 0;
};

}