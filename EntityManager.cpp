#include "EntityManager.hpp"

std::string read_file(const char* filepath)
{
    std::ifstream file(filepath);
    std::string file_contents{ std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
    return file_contents;
}

uint32_t compile_shader(const char* shader_str, int shader_type)
{
    uint32_t shader_id = glCreateShader(shader_type);
    glShaderSource(shader_id, 1, &shader_str, NULL);
    glCompileShader(shader_id);

    int success;
    char info_log[512];
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

    const char* shader_type_str = "undefined";
    if (shader_type == GL_VERTEX_SHADER)
    {
        shader_type_str = "vertex";
    }
    else if (shader_type == GL_FRAGMENT_SHADER)
    {
        shader_type_str = "fragment";
    }

    if (!success)
    {
        glGetShaderInfoLog(shader_id, 512, NULL, info_log);
        std::string error = std::format("Error, {} shader failed to compile\n{}", shader_type_str, info_log);
        throw std::runtime_error(error);
    }

    return shader_id;
}

uint32_t create_shader_program(const char* vertex_shader_source, const char* fragment_shader_source)
{
    std::string  vertex_shader_str   = read_file(vertex_shader_source);
    std::string  fragment_shader_str = read_file(fragment_shader_source);
    uint32_t     vertex_shader       = compile_shader(vertex_shader_str.c_str(), GL_VERTEX_SHADER);
    uint32_t     fragment_shader     = compile_shader(fragment_shader_str.c_str(), GL_FRAGMENT_SHADER);
    uint32_t     shader_program      = glCreateProgram();

    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    int success;
    char info_log[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        std::string error = std::format("Shader program failed to link.\n{}", info_log);
        throw std::runtime_error(error);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
}

const uint32_t ecs::EntityManager::new_entity()
{
    if (free_ids.empty())
    {
        entity_counter++;
        uint32_t id = next_id;
        next_id++;
        return id;
    }
    else
    {
        entity_counter++;
        uint32_t id = free_ids.back();
        free_ids.pop_back();
        return id;
    }
}

void ecs::EntityManager::remove_entity(uint32_t entity_id)
{
    bool has_position   = positions.has(entity_id);
    bool has_collider   = colliders.has(entity_id);
    bool has_rigid_body = rigid_bodies.has(entity_id);

    bool has_mesh       = meshes.has(entity_id);

    if (has_position)
    {
        positions.remove(entity_id);
    }
    if (has_collider)
    {
        colliders.remove(entity_id);
    }
    if (has_rigid_body)
    {
        rigid_bodies.remove(entity_id);
    }
    if (has_mesh)
    {
        meshes.remove(entity_id);
    }
    if (has_position or has_collider or has_rigid_body or has_mesh)
    {
        free_ids.push_back(entity_id);
        entity_counter--;
    }
    else if (entity_id < next_id and not (std::ranges::find(free_ids, entity_id) != free_ids.end()))
    {
        free_ids.push_back(entity_id);
        entity_counter--;
    }
    else
    {
        throw std::runtime_error("PhysicsEnvSingleton::remove_entity() failed. Entity ID doesn't exist.");
    }
}


void ecs::EntityManager::attach_position(uint32_t entity_id, const Position& position)
{
    positions.add(entity_id, position);
}

void ecs::EntityManager::attach_collider(uint32_t entity_id, ColliderType collider_type)
{
    colliders.add(entity_id, collider_type);
}

void ecs::EntityManager::attach_rigid_body(uint32_t entity_id, const RigidBody& rigid_body)
{
    rigid_bodies.add(entity_id, rigid_body);
}

void ecs::EntityManager::attach_mesh(uint32_t entity_id, Mesh mesh)
{
    meshes.add(entity_id, mesh);
}


// We can make this faster by precalculating which Entities have both position and rigidbody, in a vector.
void ecs::EntityManager::physics_step(float delta_time)
{
    glm::vec3 gravity = glm::vec3(0, -9.81f, 0);

    size_t i = 0;
    for (RigidBody& body : rigid_bodies.get_dense())
    {
        uint32_t entity_id = rigid_bodies.get_associated_handle(i);
        if (positions.has(entity_id))
        {
            Position& position = positions.get(entity_id);

            // Accumulate gravity as a force. other forces (like thrust,
            // collisions, etc.) are added elsewhere
            body.force += body.mass * gravity;

            // Newton's second law: a = f/m
            // Integrates acceleration into velocity. This is the semi - implicit
            // Euler method, which is more stable than simple(explicit) Euler.
            if (body.mass != 0.0f) // avoid diving my zero if the mass is 0
            {
                body.velocity += (body.force / body.mass) * delta_time;
            }

            position.value += body.velocity / delta_time;
            body.force = glm::vec3(0.0f);
        }
        i++;
    }
}

void ecs::EntityManager::debug_objects()
{
    size_t i = 0;
    for (RigidBody& body : rigid_bodies.get_dense())
    {
        if (positions.has(rigid_bodies.get_associated_handle(i)))
        {
            std::cout << "Entity ID: " << std::to_string(i) << std::endl;

            Position& position = positions.get(i);
            std::string message1 = std::format("Position: ({}, {}, {})", position.value.x, position.value.y, position.value.z);
            std::cout << message1 << std::endl;

            std::string message2 = std::format("Velocity: ({}, {}, {})m/s",
                body.velocity.x, body.velocity.y, body.velocity.z);
            std::cout << message2 << std::endl << std::endl;
        }
        i++;
    }
}

void ecs::EntityManager::draw()
{
    for (Mesh& mesh : meshes.get_dense())
    {
        glUseProgram(mesh.shader);
        glBindVertexArray(mesh.vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}