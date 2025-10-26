#pragma once

#include "Mesh.hpp"
#include "ID.hpp"

namespace core {

/// <summary>
/// ID for referencing models.
/// </summary>
struct ModelID : public core::ID<ModelID> {
    using core::ID<ModelID>::ID;
};

/// <summary>
/// A model is the representation of a complete 3D object composed of one or more meshes.
/// </summary>
struct Model {
    std::vector<MeshID> meshes;
};

}

DEFINE_ID_HASH(core::ModelID)