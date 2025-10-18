#include "LowLevelRenderer.hpp"

core::LowLevelRenderer::LowLevelRenderer
(
    std::shared_ptr<MeshRegistry>        mesh_registry,
    std::shared_ptr<core::PhysicsSystem> physics_system
)
    :
    mesh_registry(std::move(mesh_registry)),
    physics_system(std::move(physics_system))
{
}

void core::LowLevelRenderer::draw_geometry
(
    const Mesh&      mesh,
    const glm::vec3& position,
    const Material&  material,
    const Camera&    camera
)
{
#ifdef _DEBUG
    utils::time_and_name_log(__FUNCTION__);
    std::cout << std::format("mesh=[{}, {}, {}]", mesh.vbo, mesh.vao, mesh.ebo) << std::endl;
    std::cout << std::format("position=[{}, {}, {}]", position.x, position.y, position.z) << std::endl;
    std::cout << std::format("material=[{}]", material.shader_id) << std::endl;
#endif

    glm::mat4 model_matrix = glm::mat4(1.0f);
    model_matrix = glm::translate(model_matrix, position);

    uint32_t model_location = glGetUniformLocation(material.shader_id, "model");
    glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model_matrix));

    glm::mat4 view_matrix = glm::mat4(1.0f);

    view_matrix = glm::lookAt(camera.position, camera.target, camera.up);

    uint32_t view_location = glGetUniformLocation(material.shader_id, "view");
    glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view_matrix));

    glm::mat4 projection_matrix =
        glm::perspective(camera.fov, camera.aspect_ratio, camera.near_clip_plane, camera.far_clip_plane);

    uint32_t projection_location = glGetUniformLocation(material.shader_id, "projection");
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection_matrix));

    glUseProgram(material.shader_id);
    glBindVertexArray(mesh.vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

core::CameraID core::LowLevelRenderer::add_camera(const glm::vec3& position, const glm::vec3& up)
{
#ifdef _DEBUG
    utils::time_and_name_log(__FUNCTION__);
    std::cout << std::format("position=glm::vec3({}, {}, {}),", position.x, position.y, position.z) << std::endl;
    std::cout << std::format("up=glm::vec3({}, {}, {}),", up.x, up.y, up.z) << std::endl;
#endif

    glm::vec3 target            = glm::vec3(0.0f);
    glm::vec3 reverse_direction = glm::normalize(position - target);
    glm::vec3 right             = glm::normalize(glm::cross(up, reverse_direction));
    float near_clip_plane       = 0.1f;
    float far_clip_plane        = 100.0f;
    float aspect_ratio          = 16.0f / 9.0f;
    float fov                   = glm::radians(72.0f); // always in radians

    core::Camera camera
    {
        position,
        target,
        reverse_direction,
        up,
        right,
        near_clip_plane,
        far_clip_plane,
        aspect_ratio,
        fov
    };

    return CameraID(cameras.add(camera));
}

core::Camera& core::LowLevelRenderer::get_camera(const CameraID id)
{
#ifdef _DEBUG
    if (!cameras.has(id))
    {
        throw std::runtime_error("core::LowLevelRenderer::get_camera() failed. No camera with this id exists.");
    }
#endif

    return cameras.get(id);
}

void core::LowLevelRenderer::remove_camera(const CameraID id)
{
#ifdef _DEBUG
    utils::time_and_name_log(__FUNCTION__);
    std::cout << "id=" << id << std::endl;

    if (!cameras.has(id))
    {
        throw std::runtime_error("core::LowLevelRenderer::remove_camera() failed. No camera with this id exists.");
    }
#endif

    cameras.remove(id);
}
