#include "Renderer.hpp"

core::Renderer::Renderer(std::shared_ptr<ServiceLocator> locator) : locator(std::move(locator)) {}

void core::Renderer::draw_geometry(
    const Mesh& mesh,
    const glm::vec3& position,
    const Material& material,
    const Camera& camera)
{
#ifdef _DEBUG
    utils::time_and_name_log(__FUNCTION__);
    std::cout << std::format("mesh=[vbo={}, vao={}, ebo={}]", mesh.vbo, mesh.vao, mesh.ebo) << std::endl;
    utils::print_vec3(position, "position");
    std::cout << std::format("shader_program_id={}", material.shader_program_id) << std::endl;
#endif

    glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), position);
    uint32_t  model_location = glGetUniformLocation(material.shader_program_id, "model");
    glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model_matrix));

    glm::mat4 view_matrix = camera.get_look_at();
    uint32_t  view_location = glGetUniformLocation(material.shader_program_id, "view");
    glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view_matrix));

    glm::mat4 projection_matrix = camera.get_perspective();
    uint32_t  projection_location = glGetUniformLocation(material.shader_program_id, "projection");
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection_matrix));

    glUseProgram(material.shader_program_id);
    glBindVertexArray(mesh.vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
