#pragma once
#include "SparseSet.hpp"

#include <stdexcept>
#include <iostream>
#include <format>

#include <glm/glm.hpp>


namespace phys
{

struct StaticID
{
    uint32_t value;
    // Allows use like a uint32_t
    operator uint32_t() const
    {
        return value;
    }
};

struct DynamicID
{
    uint32_t value;
    // Allows use like a uint32_t
    operator uint32_t() const
    {
        return value;
    }
};

struct DynamicObject
{
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 force;
    float     mass;
};

struct StaticObject
{
    glm::vec3 position;
};

class PhysicsSystem
{
    private:
    SparseSet<DynamicObject> dynamic_objects;
    SparseSet<StaticObject>  static_objects;

    glm::vec3                gravity          = glm::vec3(0, -9.806f, 0);

    public:
    StaticObject& get_static(StaticID id);
        
    DynamicObject& get_dynamic(DynamicID id);

    StaticID add_static(const glm::vec3& pos);

    DynamicID add_dynamic(const glm::vec3& pos, const glm::vec3& vel, const glm::vec3& f, float m);

    void remove_static(StaticID id);

    void remove_dynamic(DynamicID id);

    void step(float delta_time);

    void debug_objects();
};

}