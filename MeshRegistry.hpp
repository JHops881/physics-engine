#pragma once
#include "SparseSet.hpp"
#include <glm/glm.hpp>
#include <stdexcept>

namespace core
{

struct MeshID
{
    uint32_t value;
    // Allows use like a uint32_t
    operator uint32_t() const
    {
        return value;
    }
};

struct Mesh {
    uint32_t vbo;
    uint32_t vao;
    uint32_t ebo;
    uint32_t shader;
};

class MeshRegistry
{
    private:
    SparseSet<Mesh> meshes{};

    public:

    MeshID add_mesh(Mesh mesh);

    void remove_mesh(MeshID id);

    Mesh& get_mesh(MeshID id);
};

}