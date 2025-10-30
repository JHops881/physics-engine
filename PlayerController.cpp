#include "PlayerController.hpp"

core::PlayerController::PlayerController(GLFWwindow* window)
    : window(window),
      last_mouse_pos(glm::vec2(0.0f))
{   
    // initialize the last mouse position to the center of the screen.
    GLfloat viewport_config[4]; // 4 values: x (0), y (1), width (2), height (3)
    glGetFloatv(GL_VIEWPORT, viewport_config);
    last_mouse_pos.x = viewport_config[2] / 2.0f;
    last_mouse_pos.y = viewport_config[3] / 2.0f;
}

void core::PlayerController::process_input(core::Camera& camera, float delta_time) {
    // Exiting the application
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    // New direction with mouse movement.
    double mouse_current_pos_x;
    double mouse_current_pos_y;
    glfwGetCursorPos(window, &mouse_current_pos_x, &mouse_current_pos_y);

    float offset_x = static_cast<float>(mouse_current_pos_x) - last_mouse_pos.x;
    float offset_y = static_cast<float>(mouse_current_pos_y) - last_mouse_pos.y;

    last_mouse_pos.x = static_cast<float>(mouse_current_pos_x);
    last_mouse_pos.y = static_cast<float>(mouse_current_pos_y);

    float sensitivity = 0.1f;
    offset_x *= sensitivity;
    offset_y *= sensitivity * -1.0f; // gotta flip it cause GLFW coords start in top left

    float y = camera.get_yaw() + offset_x;
    float p = camera.get_pitch() + offset_y;

    // Little bit of correction so we don't look straight up and around.
    if (p > 89.0f) {
        p = 89.0f;
    }
    if (p < -89.0f) {
        p = -89.0f;
    }

    camera.set_rotation(p, y);

    // Camera movement with wasd
    glm::vec3 offset = glm::vec3(0.0f);
    bool moved = false;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        offset += camera.get_front();
        moved = true;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        offset += -camera.get_right();
        moved = true;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        offset += -camera.get_front();
        moved = true;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        offset += camera.get_right();
        moved = true;
    }
    float speed = 6.0f;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        speed *= 2.0f;
    }
    if (moved and offset != glm::vec3(0.0f)) { // avoid dividing by 0.
        // Normalize the vector, so angular movement isn't faster.
        offset = glm::normalize(offset);
        camera.set_postition(camera.get_position() + offset * speed * delta_time);
    }

}
