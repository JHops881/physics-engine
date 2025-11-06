#pragma once
#include "ID.hpp"

namespace core {

/// <summary>
/// And ID that references a material
/// </summary>
struct MaterialID : public core::ID<MaterialID> {
    using core::ID<MaterialID>::ID;
};

/// <summary>
/// Defines what the surface of a mesh should look like.
/// </summary>
struct Material {
    glm::vec3 color;
    GLfloat ambient_strength;
    GLfloat specular_strength;
    GLfloat shininess;
};

}

DEFINE_ID_HASH(core::MaterialID)