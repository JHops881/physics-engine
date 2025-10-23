#include "SceneRenderingManager.hpp"

core::SceneRenderingManager::SceneRenderingManager(std::shared_ptr<ServiceLocator> locator) : locator(locator) {}

void core::SceneRenderingManager::init_skybox_geometry() {
    std::vector<GLfloat> vertex_data{
        // positions 
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
       -0.5f, -0.5f, -0.5f,
       -0.5f,  0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
       -0.5f, -0.5f,  0.5f,
       -0.5f,  0.5f,  0.5f,
    };
    std::vector<GLushort> indices{
      3, 2, 1, 1, 4, 3,
      6, 5, 0, 0, 7, 6,
      7, 4, 3, 3, 6, 7,
      0, 1, 2, 2, 5, 0,
      3, 2, 5, 5, 6, 3,
      4, 1, 0, 0, 7, 4
    };
    auto renderer_3d = locator->get_service<core::IRenderer3D>();

    const char* vertex_shader_filepath = "shaders/cubemap.vert";
    const char* fragment_shader_filepath = "shaders/cubemap.frag";
    skybox_shader = renderer_3d->new_shader_program(vertex_shader_filepath, fragment_shader_filepath);

    GLuint VBO = renderer_3d->new_VBO(vertex_data);
    GLuint EBO = renderer_3d->new_EBO(indices);
    skybox_vao = renderer_3d->new_VAO(VBO, EBO, 1, { 3 });
}

void core::SceneRenderingManager::render_skybox(const Camera& camera) const {
    // remember, the skybox is "all in your head"
    auto renderer_3d = locator->get_service<core::IRenderer3D>();
    auto scene_manager = locator->get_service<core::ISceneManager>();
    GLuint skybox = scene_manager->get_skybox();
    glDepthMask(GL_FALSE);
    renderer_3d->draw_indexed_geometry(skybox_vao, skybox_shader, skybox, 36, camera.get_position(), camera);
    glDepthMask(GL_TRUE);
}
