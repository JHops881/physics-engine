#pragma once

#include "ID.hpp"
#include "Texture.hpp"

namespace core {

/// <summary>
/// And ID that references a material
/// </summary>
struct MaterialID : public core::ID<MaterialID> {
    using core::ID<MaterialID>::ID;
};

/// <summary>
/// Defines what the surface of a model should look like. A coupling of both the texture and shader.
/// </summary>
struct Material {
    TextureID texture_id;
    unsigned int shader_program_id;
};

}

DEFINE_ID_HASH(core::MaterialID)