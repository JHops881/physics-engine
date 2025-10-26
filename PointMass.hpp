#pragma once
#include "ID.hpp"
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

}
