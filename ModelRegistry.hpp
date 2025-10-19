#pragma once
#include "SparseSet.hpp"
#include "MeshRegistry.hpp"
#include "ID.hpp"

#include <glm/glm.hpp>
#include <stdexcept>

namespace core {

struct Material { // TODO: a material is a shaderID and a textureID.
    uint32_t shader_id;
    uint32_t placeholder;
};

struct Model {
    MeshID mesh_id;
    Material material; // TODO: actually, this needs to be a MaterialID.
};

struct ModelID : public core::ID<ModelID> {
    using core::ID<ModelID>::ID;
};

class ModelRegistry {
private:
    SparseSet<Model> models{};
public:
    ModelID add_model(Model model);

    void remove_model(ModelID id);

    Model& get_model(ModelID id);
};

}