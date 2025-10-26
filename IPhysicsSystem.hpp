#pragma once
#include "utils.hpp"
#include "PointMass.hpp"
#include "ServiceLocator.hpp"

#include <glm/glm.hpp>

namespace core {

/// <summary>
/// A System that computes the motion, collisions, and physical parameters of objects that exist within the simulated world. 
/// </summary>
class IPhysicsSystem : public IService {
public:
    virtual ~IPhysicsSystem() = default;
    /// <summary>
    /// Retrieve a point mass in exchange for its ID.
    /// </summary>
    /// <param name="id">: ID of the point mass</param>
    /// <returns>Reference to the point mass</returns>
    virtual PointMass& get_point_mass(PointMassID id) = 0;

    /// <summary>
    /// Add a point mass to the physics environment.
    /// </summary>
    /// <param name="pos">: Position of the point mass in 3D space</param>
    /// <returns>ID of the point mass (used to reference it later)</returns>
    virtual PointMassID add_point_mass(const glm::vec3& pos) = 0;

    /// <summary>
    /// Remove a point mass from the physics environment.
    /// </summary>
    /// <param name="id">ID of the point mass that you want to remove</param>
    virtual void remove_point_mass(PointMassID id) = 0;

    /// <summary>
    /// Advance the physics simulation by one simulation step.
    /// </summary>
    /// <param name="delta_time">: The duration of time to simulate passing within this step</param>
    virtual void step(float delta_time) = 0;

    /// <summary>
    /// Print out debug information for all of the current point masses in the physics environment.
    /// </summary>
    virtual void debug_point_masses() = 0;
};

}