#pragma once
#include "ID.hpp"
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>

namespace core {

/// <summary>
/// ID for referencing meshes.
/// </summary>
struct MeshID : public core::ID<MeshID> {
    using core::ID<MeshID>::ID;
};

/// <summary>
/// Collection of vertex data that defines a 3D geometry.
/// </summary>
struct Mesh {
    std::vector<GLfloat> vertices;
    std::vector<GLushort> indices;
    GLuint texture;
};

}

DEFINE_ID_HASH(core::MeshID)