#include "MeshRegistry.hpp"
    
gfx::MeshID gfx::MeshRegistry::add_mesh(Mesh mesh)
{
    // TODO: verify that all mesh values are valid. 
    MeshID id = MeshID(meshes.add(mesh));
    return id;
}

void gfx::MeshRegistry::remove_mesh(MeshID id)
{
    if (meshes.has(id))
    {
        meshes.remove(id);
    }
    else
    {
        throw std::runtime_error("gfx::MeshRegistry::remove_mesh() failed. No mesh with this MeshID exists to remove!");
    }
}

gfx::Mesh& gfx::MeshRegistry::get_mesh(MeshID id)
{
    if (meshes.has(id))
    {
        return meshes.get(id);
    }
    else
    {
        throw std::runtime_error("gfx::MeshRegistry::get_mesh() failed. No mesh with this MeshID exists to get!");
    }
}  
