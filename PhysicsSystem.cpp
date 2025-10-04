#include "PhysicsSystem.hpp"

phys::StaticObject& phys::PhysicsSystem::get_static(StaticID id)
{
    if (static_objects.has(id))
    {
        return static_objects.get(id);
    }
    else
    {
        throw std::runtime_error
        (
            "phys::PhysicsSystem::get_static() failed. Given StaticID does not refer to an existing static object."
        );
    }
}

phys::DynamicObject& phys::PhysicsSystem::get_dynamic(DynamicID id)
{
    if (dynamic_objects.has(id))
    {
        return dynamic_objects.get(id);
    }
    else
    {
        throw std::runtime_error
        (
            "phys::PhysicsSystem::get_dynamic() failed. Given DynamicID does not refer to an existing dynamic object."
        );
    }
}

phys::StaticID phys::PhysicsSystem::add_static(const glm::vec3& pos)
{
    StaticID id = StaticID(static_objects.add({pos}));
    return id;
}

phys::DynamicID phys::PhysicsSystem::add_dynamic(const glm::vec3& pos, const glm::vec3& vel, const glm::vec3& f, float m)
{
    DynamicID id = DynamicID(dynamic_objects.add({pos, vel, f, m}));
    return id;
}

void phys::PhysicsSystem::remove_static(StaticID id)
{
    if (static_objects.has(id))
    {
        static_objects.remove(id);
    }
    else
    {
        throw std::runtime_error
        (
            "phys::PhysicsSystem::remove_static() failed. Given StaticID does not refer to an existing static object."
        );
    }
}

void phys::PhysicsSystem::remove_dynamic(DynamicID id)
{
    if (dynamic_objects.has(id))
    {
        dynamic_objects.remove(id);
    }
    else
    {
        throw std::runtime_error
        (
            "phys::PhysicsSystem::remove_dynamic() failed. Given DynamicID does not refer to an existing dynamic object."
        );
    }
}

void phys::PhysicsSystem::step(float delta_time)
// Let's cook this bad boy up with CUDA to accelerate the computing
{
    for (DynamicObject& dyn_obj : dynamic_objects.get_dense())
    {
        // Accumulate gravity as a force. other forces (like thrust,
        // collisions, etc.) are added elsewhere
        dyn_obj.force += dyn_obj.mass * gravity;

        // Newton's second law: a = f/m
        // Integrates acceleration into velocity. This is the semi - implicit
        // Euler method, which is more stable than simple(explicit) Euler.
        if (dyn_obj.mass != 0.0f) // avoid diving my zero if the mass is 0
        {
            dyn_obj.velocity += (dyn_obj.force / dyn_obj.mass) * delta_time;
        }

        dyn_obj.position += dyn_obj.velocity * delta_time;
        dyn_obj.force = glm::vec3(0.0f);

    }
}

void phys::PhysicsSystem::debug_objects()
{
    size_t i = 0;
    for (auto& dyn_obj : dynamic_objects.get_dense())
    {
        DynamicID id = DynamicID(dynamic_objects.get_associated_handle(i));
        std::cout << "DynamicID: " << std::to_string(id) << std::endl;

        std::string message1 = std::format("Position: ({}, {}, {})", dyn_obj.position.x, dyn_obj.position.y, dyn_obj.position.z);
        std::cout << message1 << std::endl;

        std::string message2 = std::format("Velocity: ({}, {}, {})m/s",
            dyn_obj.velocity.x, dyn_obj.velocity.y, dyn_obj.velocity.z);
        std::cout << message2 << std::endl << std::endl;

        i++;
    }

}