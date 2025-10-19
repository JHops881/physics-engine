#include "PhysicsSystem.hpp"

core::PointMass& core::PhysicsSystem::get_point_mass(PointMassID id) {
#ifdef _DEBUG
    if (!point_masses.has(id)) {
        throw std::runtime_error("core::PhysicsSystem::get_point_mass() failed. PointMassID doesn't exist.");
    }
#endif
    return point_masses.get(id);
}

core::PointMassID core::PhysicsSystem::add_point_mass(const glm::vec3& pos) {
#ifdef _DEBUG
    utils::time_and_name_log(__FUNCTION__);
    utils::print_vec3(pos, "pos");
#endif
    // defualt particle values
    core::PointMass point_mass {
        pos,
        glm::vec3(0.0f),
        glm::vec3(0.0f),
        glm::vec3(0.0f, -21.0f, 0.0f),
        0.995,
        1.0f
    };
    PointMassID id = PointMassID(point_masses.add(point_mass));
    return id;
}

void core::PhysicsSystem::remove_point_mass(PointMassID id) {
#ifdef _DEBUG
    utils::time_and_name_log(__FUNCTION__);
    std::cout << "id=" << id << std::endl;
    if (!point_masses.has(id)) {
        throw std::runtime_error("core::PhysicsSystem::remove_point_mass() failed. PointMassID doesn't exist.");
    }
#endif
    point_masses.remove(id);
}

void core::PhysicsSystem::step(float delta_time) {
// Let's cook this bad boy up with CUDA to accelerate the computing
#ifdef _DEBUG
    utils::time_and_name_log(__FUNCTION__);
    std::cout << "delta_time=" << delta_time << std::endl;
    auto start_time = std::chrono::high_resolution_clock::now();
#endif
    for (PointMass& p : point_masses.get_dense()) {
        p.acceleration = p.inverse_mass * p.f_gravity;
        p.velocity += p.acceleration * delta_time;

        // p' = p + vt + 1/2at^2
        p.position = p.position + p.velocity * p.dampening * delta_time + p.acceleration * delta_time * delta_time * 0.5f;
    }
#ifdef _DEBUG
    auto end_time = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration<double>(end_time - start_time).count();
    std::cout << std::fixed << "Update took " << duration * 1000.0 << "ms" << std::endl;
    debug_point_masses();
#endif
}

void core::PhysicsSystem::debug_point_masses() {
    std::cout << "Alive PointMasses: " << std::endl;
    size_t i = 0;
    for (auto& p : point_masses.get_dense()) {
        PointMassID id = PointMassID(point_masses.get_associated_handle(i));
        std::cout << "PointMassID: " << std::to_string(id) << std::endl;
        utils::print_vec3(p.position, "Position");
        utils::print_vec3(p.velocity, "Velocity");
        i++;
    }
}