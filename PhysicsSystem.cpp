#include "PhysicsSystem.hpp"

core::Particle& core::PhysicsSystem::get_particle(ParticleID id)
{
    if (particles.has(id))
    {
        return particles.get(id);
    }
    else
    {
        throw std::runtime_error
        (
            "phys::PhysicsSystem::get_particle() failed. Given DynamicID does not refer to an existing dynamic object."
        );
    }
}

core::ParticleID core::PhysicsSystem::add_particle(const glm::vec3& pos)
{
    // defualt particle values
    core::Particle particle
    {
        pos,
        glm::vec3(0.0f),
        glm::vec3(0.0f),
        glm::vec3(0.0f, -21.0f, 0.0f),
        0.995,
        1.0f
    };
    ParticleID id = ParticleID(particles.add(particle));
    return id;
}

void core::PhysicsSystem::remove_particle(ParticleID id)
{
    if (particles.has(id))
    {
        particles.remove(id);
    }
    else
    {
        throw std::runtime_error
        (
            "phys::PhysicsSystem::remove_particle() failed. Given DynamicID does not refer to an existing dynamic object."
        );
    }
}

void core::PhysicsSystem::step(float delta_time)
// Let's cook this bad boy up with CUDA to accelerate the computing
{
    double start_time = std::chrono::duration<double>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

    for (Particle& p : particles.get_dense())
    {
        p.acceleration = p.inverse_mass * p.f_gravity;
        p.velocity += p.acceleration * delta_time;

        // p' = p + vt + 1/2at^2
        p.position = p.position + p.velocity * p.dampening * delta_time + p.acceleration * delta_time * delta_time * 0.5f;
    }

    double end_time = std::chrono::duration<double>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    double duration = end_time - start_time;
    std::cout << std::to_string(duration * (double)1000) << "ms" << std::endl;
}

void core::PhysicsSystem::debug_particles()
{
    size_t i = 0;
    for (auto& p : particles.get_dense())
    {
        ParticleID id = ParticleID(particles.get_associated_handle(i));
        std::cout << "DynamicID: " << std::to_string(id) << std::endl;

        std::string message1 = std::format("Position: ({}, {}, {})", p.position.x, p.position.y, p.position.z);
        std::cout << message1 << std::endl;

        std::string message2 = std::format("Velocity: ({}, {}, {})m/s",
            p.velocity.x, p.velocity.y, p.velocity.z);
        std::cout << message2 << std::endl << std::endl;

        i++;
    }
}