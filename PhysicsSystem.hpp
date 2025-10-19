#pragma once
#include "SparseSet.hpp"
#include "utils.hpp"
#include "ID.hpp"

#include <stdexcept>
#include <iostream>
#include <format>
#include <vector>
#include <chrono>

#include <glm/glm.hpp>


namespace core {

/// <summary>
/// An ID for a PointMass that is used to identify it within the physics system.
/// </summary>
struct PointMassID : public core::ID<PointMassID> {
    using core::ID<PointMassID>::ID;
};

/// <summary>
/// A point in 3 dimensional space that has mass and is affected by forces and the like. Building block for larger
/// physical structures in a mass-aggregate physics system.
/// </summary>
struct PointMass {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    // Every PointMass gets its own gravity for more exciting simulation
    glm::vec3 f_gravity;
    // Represents the effect spacial dampening like drag have on the object.
    // Is a percentage of how much velocity is conserved each second. 
    float     dampening;
    /*
    Because: acceleration = 1/mass * force, and our physics engine applies forces to objects,
    then we can skip finding the inverse. Also, this allows us to represent infinte mass 
    for immovable objects.
    */
    float     inverse_mass;
};

/// <summary>
/// A System that computes the motion, collisions, and physical parameters of objects that exist within the simulated world. 
/// </summary>
class PhysicsSystem {
private:
    SparseSet<PointMass> point_masses;
public:
    /// <summary>
    /// Retrieve a point mass in exchange for its ID.
    /// </summary>
    /// <param name="id">: ID of the point mass</param>
    /// <returns>Reference to the point mass</returns>
    PointMass& get_point_mass(PointMassID id);

    /// <summary>
    /// Add a point mass to the physics environment.
    /// </summary>
    /// <param name="pos">: Position of the point mass in 3D space</param>
    /// <returns>ID of the point mass (used to reference it later)</returns>
    PointMassID add_point_mass(const glm::vec3& pos);

    /// <summary>
    /// Remove a point mass from the physics environment.
    /// </summary>
    /// <param name="id">ID of the point mass that you want to remove</param>
    void remove_point_mass(PointMassID id);

    /// <summary>
    /// Advance the physics simulation by one simulation step.
    /// </summary>
    /// <param name="delta_time">: The duration of time to simulate passing within this step</param>
    void step(float delta_time);

    /// <summary>
    /// Print out debug information for all of the current point masses in the physics environment.
    /// </summary>
    void debug_point_masses();
};

}