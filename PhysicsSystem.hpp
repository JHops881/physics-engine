#pragma once
#include "SparseSet.hpp"
#include "utils.hpp"

#include <stdexcept>
#include <iostream>
#include <format>
#include <vector>
#include <chrono>

#include <glm/glm.hpp>


namespace core
{

/// <summary>
/// An ID for a PointMass that is used to identify it within the physics system.
/// </summary>
struct PointMassID
{
    uint32_t value;
    // Allows use like a uint32_t
    operator uint32_t() const
    {
        return value;
    }
};

/// <summary>
/// A point in 3 dimensional space that has mass and is affected by forces and the like. Building block for larger
/// physical structures in a mass-aggregate phsysics system.
/// </summary>
struct PointMass
{
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

class PhysicsSystem
{
    private:
    SparseSet<PointMass> particles;

    public:
    /// <summary>
    /// Get a 
    /// </summary>
    /// <param name="id"></param>
    /// <returns></returns>
    PointMass& get_point_mass(PointMassID id);

    PointMassID add_point_mass(const glm::vec3& pos);

    void remove_point_mass(PointMassID id);

    void step(float delta_time);

    void debug_point_masses();
};

}