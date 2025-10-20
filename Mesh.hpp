#pragma once
#include "ID.hpp"

namespace core {

/// <summary>
/// ID for referencing meshes.
/// </summary>
struct MeshID : public core::ID<MeshID> {
    using core::ID<MeshID>::ID;
};

/// <summary>
/// Collection of vertex data and its interpretation that defines a 3D shape.
/// </summary>
struct Mesh {
    unsigned int vbo;
    unsigned int vao;
    unsigned int ebo;
};

}

DEFINE_ID_HASH(core::MeshID)