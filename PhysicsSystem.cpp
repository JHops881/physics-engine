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
    if (m > 0.0f)
    {
        DynamicID id = DynamicID(dynamic_objects.add({ pos, vel, f, m }));
        return id;
    }
    else
    {
        throw std::runtime_error("phys::PhysicsSystem::add_dynamic() failed. Mass cannot be a value <= 0.0f");
    }
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
const glm::vec3 phys::PhysicsSystem::get_overlap(const glm::vec3& pos1, const glm::vec3 pos2, float half_width) const
{
    // https://www.youtube.com/watch?v=9QgaLWBkv0s
    // Get the volume of a potential overlap volume
    glm::vec3 delta = glm::abs(pos1 - pos2);
    float overlap_x = half_width + half_width - delta.x; // technically, this is w1/2 + w2/2 - dx
    float overlap_y = half_width + half_width - delta.y;
    float overlap_z = half_width + half_width - delta.z;

    return glm::vec3(overlap_x, overlap_y, overlap_z);
}

const bool phys::PhysicsSystem::are_colliding(phys::DynamicObject& a, phys::StaticObject b) const
{
    float half_width = 0.5f; // need to add this as an attribute of the objects

    glm::vec3 overlap = get_overlap(a.position, b.position, half_width);

    if (overlap.x > 0.0f and // Collision when the overlap volume exists in positive space
        overlap.y > 0.0f and
        overlap.z > 0.0f)
    {
        return true;
    }
    return false;
}

void phys::PhysicsSystem::step(float delta_time)
// Let's cook this bad boy up with CUDA to accelerate the computing
{
    double start_time = std::chrono::duration<double>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

    for (DynamicObject& a : dynamic_objects.get_dense())
    {
        // Accumulate gravity as a force. other forces (like thrust,
        // collisions, etc.) are added elsewhere
        a.force += a.mass * gravity;

        // Newton's second law: a = f/m
        // Integrates acceleration into velocity. This is the semi - implicit
        // Euler method, which is more stable than simple(explicit) Euler.
        a.velocity += (a.force / a.mass) * delta_time;

        glm::vec3 old_position = a.position;

        a.position += a.velocity * delta_time;

        a.force = glm::vec3(0.0f);

        for (StaticObject& b : static_objects.get_dense())
        {
            if (are_colliding(a, b))
            {
                std:: cout << "collision" << std::endl;

                float half_width = 0.5f; // need to add this as an attribute of the objects

                glm::vec3 overlap = get_overlap(a.position, b.position, half_width);

                glm::vec3 previous_overlap = get_overlap(old_position, b.position, half_width);

                float e = 0.6f; // Coefficient of Restitution
                // e = -((v1-v2)/(V1-V2))
                //float u = 0.3f; // Ff = u * N (force of friction is equals coefficient of friction * normal force)

                //float h = 0.50f;  // F = mv²/2h

                //glm::vec3 total_force = a.mass
                //    * glm::vec3(a.velocity.x * a.velocity.x, a.velocity.y * a.velocity.y, a.velocity.z * a.velocity.z)
                //    / (2.0f * h);

                if (previous_overlap.y > 0.0f and previous_overlap.z > 0.0f) // X FACE COLLISIONS
                {
                    if (a.position.x > b.position.x)      // Left Face Collision
                    {
                        a.position.x += overlap.x;
                    }
                    else if (a.position.x < b.position.x) // Right Face Collision
                    {
                        a.position.x -= overlap.x;
                    }
                    else
                    {
                        // TODO
                    }
                    a.velocity.x = -a.velocity.x * e;
                    //float N = -total_force.x;
                    //std:: cout << "Normal Force " << N << std::endl;
                    //std:: cout << "warning!" << std::endl;
                    //float Ff = u * N;
                    //a.force.y += Ff;

                }
                else if (previous_overlap.x > 0.0f and previous_overlap.z > 0.0f) // Y FACE COLLISIONS
                {

                    if (a.position.y > b.position.y) // Bottom Face
                    {
                        a.position.y += overlap.y;

                    }
                    else if (a.position.y < b.position.y) // Top Face
                    {
                        a.position.y -= overlap.y;
                    }
                    else
                    {
                        // TODO
                    }
                    a.velocity.y = -a.velocity.y * e;

                    //float N = 0.0f;
                    //if (a.velocity.y > 0)
                    //{
                    //    N = -total_force.y;
                    //}
                    //else
                    //{
                    //    N = total_force.y;
                    //}
                    //a.force.x += N;

                    //float Ff = u * N;
                    //a.force.x += Ff;

                    //std::cout << "Normal Force " << N << std::endl;
                    //std::cout << a.velocity.y << std::endl;
                    //std::string message1 = std::format(" force x,y,z: {},{},{}", a.force.x, a.force.y, a.force.z);
                    //std::cout << message1 << std::endl;
                }
                else if (previous_overlap.y > 0.0f and previous_overlap.x > 0.0f) // Z FACE COLLISIONS
                {
                    if (a.position.z > b.position.z) // Back Face Collision
                    {
                        a.position.z += overlap.z;
                    }
                    else if (a.position.z < b.position.z) // Front Face Collision
                    {
                        a.position.z -= overlap.z;
                    }
                    else
                    {
                        // TODO
                    }
                    a.velocity.z = -a.velocity.z * e;
                }
                else if (previous_overlap.x <= 0.0f and // no x or y overlap previously,   //  right and left edges
                    previous_overlap.y <= 0.0f and
                    previous_overlap.z > 0.0f)
                {
                    if (a.position.x > b.position.x)
                    {
                        if (a.position.y < b.position.y) // Top left edge
                        {
                            // Default to moving down.
                            a.position.y -= overlap.y;
                        }
                        else if (a.position.y > b.position.y) // bottom left edge
                        {
                            // Default to moving up.
                            a.position.y += overlap.y;
                        }

                    }
                    else if (a.position.x < b.position.x)
                    {
                        if (a.position.y < b.position.y) // Top right edge
                        {
                            // Default to moving down.
                            a.position.y -= overlap.y;
                        }
                        else if (a.position.y > b.position.y) // bottom right edge
                        {
                            // Default to moving up.
                            a.position.y += overlap.y;
                        }
                    }
                    a.velocity.y = -a.velocity.y * e;
                }
                else if (previous_overlap.z <= 0.0f and // no x or y overlap previously,   //  right and left edges
                         previous_overlap.y <= 0.0f and
                         previous_overlap.x > 0.0f)
                {
                    if (a.position.z > b.position.z)
                    {
                        if (a.position.y < b.position.y) // Top front edge
                        {
                            // Default to moving down.
                            a.position.y -= overlap.y;
                        }
                        else if (a.position.y > b.position.y) // bottom front edge
                        {
                            // Default to moving up.
                            a.position.y += overlap.y;
                        }
                    }
                    else if (a.position.z < b.position.z)
                    {
                        if (a.position.y < b.position.y) // Top back edge
                        {
                            // Default to moving down.
                            a.position.y -= overlap.y;
                        }
                        else if (a.position.y > b.position.y) // bottom back edge
                        {
                            // Default to moving up.
                            a.position.y += overlap.y;
                        }
                    }
                    a.velocity.y = -a.velocity.y * e;

                }
                // TODO those 4 vertical edges
                else
                {
                    std::string message1 = std::format("Unplanned situtation. previous overlap x,y,z: {},{},{}", previous_overlap.x, previous_overlap.y, previous_overlap.z);
                    std::string message2 = std::format("Unplanned situtation. overlap x,y,z: {},{},{}", overlap.x, overlap.y, overlap.z);
                    std::cout << message1 << std::endl;
                    std::cout << message2 << std::endl;
                    throw std::runtime_error("Need to implement!");
                }
            }
        }

    }
    double end_time = std::chrono::duration<double>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    double duration = end_time - start_time;
    std::cout << std::to_string(duration * (double)1000) << "ms" << std::endl;
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