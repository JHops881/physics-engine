#pragma once
#include "Camera.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace core {

/// <summary>
/// An interface for translating player input into in-game actions.
/// </summary>
class PlayerController {
private:
    GLFWwindow* window;
    glm::vec2 last_mouse_pos;
public:
    /// <summary>
    /// Create a new player controller. 
    /// </summary>
    /// <param name="window"></param>
    PlayerController(GLFWwindow* window);

    /// <summary>
    /// Main function for turning key input into in-game actions. 
    /// </summary>
    void process_input(core::Camera& camera, float delta_time);
};

}
