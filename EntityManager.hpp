#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <cstdint>
#include <vector>
#include <glm/glm.hpp>
#include <format>
#include <iostream>
#include <fstream>

#include "SparseSet.hpp"

std::string read_file(const char* filepath);

uint32_t compile_shader(const char* shader_source, int shader_type);

uint32_t create_shader_program(const char* vertex_shader_source, const char* fragment_shader_source);

namespace ecs
{

// Components
struct Position
{
    glm::vec3 value;
};

/// <summary>
/// 3D shape of a Collider's bounding box
/// </summary>
enum ColliderType
{
    CUBE,
    SPHERE
};

struct RigidBody
{
    glm::vec3 velocity;
    glm::vec3 force;
    float     mass;
};

struct Mesh {
    uint32_t vbo;
    uint32_t vao;
    uint32_t ebo;
    uint32_t shader;
};

class EntityManager
{
  private:
    std::vector<uint32_t>   free_ids{};       // stores retired entity ids that can be reused
    uint32_t                next_id           = 0;
    uint32_t                entity_counter    = 0;

    // physics
    SparseSet<Position>     positions{};
    SparseSet<ColliderType> colliders{};
    SparseSet<RigidBody>    rigid_bodies{};

    // gfx
    SparseSet<Mesh>         meshes;

  public:
    const uint32_t new_entity();

    void remove_entity(uint32_t entity_id);

    // Physics

    void attach_position(uint32_t entity_id, const Position& position);

    void attach_collider(uint32_t entity_id, ColliderType collider_type);

    void attach_rigid_body(uint32_t entity_id, const RigidBody& rigid_body);

    void attach_mesh(uint32_t entity_id, Mesh mesh);

    /// <summary>
    /// Advance the physics simulation by one simulation step.
    /// </summary>
    /// <param name="delta_time">: time elapsed since the last update</param>
    void physics_step(float delta_time);

    // gfx

    void draw();

    void debug_objects();
};

} // namespace ecs