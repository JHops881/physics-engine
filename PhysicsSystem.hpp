#pragma once
#include "SparseSet.hpp"

#include <stdexcept>
#include <iostream>
#include <format>
#include <vector>
#include <chrono>

#include <glm/glm.hpp>


namespace core
{

struct ParticleID
{
    uint32_t value;
    // Allows use like a uint32_t
    operator uint32_t() const
    {
        return value;
    }
};

struct Particle
{
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 f_gravity;
    // Represents the effect spacial dampening like drag have on the object.
    // Is a percentage of how much velocity is conserved each second. 
    float     dampening;
    /*
    because acceleration = 1/mass * force, and our physics engine applies forces to objects,
    then we can skip finding the inverse. Also, this allows us to represent infinte mass 
    for immovable objects.
    */
    float     inverse_mass;

};

class PhysicsSystem
{
    private:
    SparseSet<Particle> particles;

    public:
    Particle& get_particle(ParticleID id);

    ParticleID add_particle(const glm::vec3& pos);

    void remove_particle(ParticleID id);

    void step(float delta_time);

    void debug_particles();
};

}