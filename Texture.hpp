#pragma once

#include "ID.hpp"

namespace core {

    /// <summary>
    /// And ID that references a texture
    /// </summary>
    struct TextureID : public core::ID<TextureID> {
        using core::ID<TextureID>::ID;
    };

    /// <summary>
    /// Defines the patterning on the surface of a model.
    /// </summary>
    struct Texture {
        unsigned int placeholder;
    };

}