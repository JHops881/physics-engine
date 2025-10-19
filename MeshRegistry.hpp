#pragma once
#include "SparseSet.hpp"
#include "ID.hpp"

#include <glm/glm.hpp>
#include <stdexcept>

namespace core {

struct MeshID : public core::ID<MeshID> {
    using core::ID<MeshID>::ID;
};

struct Mesh {
    uint32_t vbo;
    uint32_t vao;
    uint32_t ebo;
};

class MeshRegistry {
private:
    SparseSet<Mesh> meshes{};
public:
    MeshID add_mesh(Mesh mesh);

    void remove_mesh(MeshID id);

    Mesh& get_mesh(MeshID id);
};

}