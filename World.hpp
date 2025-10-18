#pragma once
#include "SparseSet.hpp"
#include "MeshRegistry.hpp"
#include "PhysicsSystem.hpp"
#include "ModelRegistry.hpp"
#include "utils.hpp"

namespace core
{

/// <summary>
/// An object that exists in the simulated world.
/// </summary>
struct WorldObject
{
    ModelID model_id;
    PointMassID pm_id;
};

/// <summary>
/// An ID for an object in the world.
/// </summary>
struct WorldObjectID
{
    uint32_t value;
    // Allows use like a uint32_t
    operator uint32_t() const
    {
        return value;
    }
};

/// <summary>
/// Simulated world that describes the scene.
/// </summary>
class World
{
  private:
    SparseSet<WorldObject> objects;   

  public:
    /// <summary>
    /// Add a new object into the world.
    /// </summary>
    /// <param name="world_object">: the object that you want to add into the world</param>
    /// <returns>ID of the object</returns>
    WorldObjectID add_object(WorldObject object);

    /// <summary>
    /// Get an object from the world.
    /// </summary>
    /// <param name="id">: ID that refers to the object</param>
    /// <returns>A reference to the object</returns>
    WorldObject& get_object(WorldObjectID id);

    /// <summary>
    /// Remove an object from the world.
    /// </summary>
    /// <param name="id">: ID that refers to the object that you want to remove</param>
    void remove_object(WorldObjectID id);

    /// <summary>
    /// Get a list of all the objects that exist in the world.
    /// </summary>
    /// <returns>A reference to a list of all the objects</returns>
    std::vector<WorldObject>& get_objects();
};

}