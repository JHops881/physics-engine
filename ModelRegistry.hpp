#pragma once
#include "SparseSet.hpp"
#include "MeshRegistry.hpp"

#include <glm/glm.hpp>
#include <stdexcept>

namespace core
{

struct Material // TODO: a material is a shaderID and a textureID.
{
    uint32_t shader_id;
    uint32_t placeholder;
};

struct Model
{
    MeshID mesh_id;
    Material material; // TODO: actually, this needs to be a MaterialID.
};

struct ModelID
{
    uint32_t value;
    // Allows use like a uint32_t
    operator uint32_t() const
    {
        return value;
    }
};

class ModelRegistry
{
private:
    SparseSet<Model> models{};

public:

    ModelID add_model(Model model);

    void remove_model(ModelID id);

    Model& get_model(ModelID id);
};

}