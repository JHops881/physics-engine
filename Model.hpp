#pragma once

#include "Mesh.hpp"
#include "ID.hpp"
#include "Material.hpp"

namespace core {

/// <summary>
/// ID for referencing models.
/// </summary>
struct ModelID : public core::ID<ModelID> {
    using core::ID<ModelID>::ID;
};

/// <summary>
/// A model is a coupling of a mesh and its appropriate material it will be rendered as.
/// </summary>
struct Model {
    MeshID mesh_id;
    MaterialID material_id;
};

}

DEFINE_ID_HASH(core::ModelID)