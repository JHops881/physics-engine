#pragma once
#include "MeshRegistry.hpp"
#include "PhysicsSystem.hpp"
#include "RenderingSystem.hpp"
#include "ShaderSystem.hpp"

#include <chrono>
#include <format>
#include <iostream>
#include <type_traits>
#include <stdexcept>
#include <cstdlib>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

constexpr uint32_t WIDTH = 1600;
constexpr uint32_t HEIGHT = 900;

/// <summary>
/// OpenGL Viewport exists inside the GLFW Window. Call this function every time
/// the GLFW window is resized, so that the viewport matches up with it.
/// </summary>
/// <param name="window">: pointer to active GLFW window with OpenGL context</param>
/// <param name="width">: (automatically passed in) new window width</param>
/// <param name="height">: (automatically passed in) new window width</param>
static void frame_buffer_size_callback(GLFWwindow* window, int width, int height);

/// <summary>
/// An application running OpenGL in a GLFW window
/// </summary>
class PhysSimApplication
{
  private:
    GLFWwindow*                           window;
    std::shared_ptr<gfx::ShaderSystem>    shader_system;
    std::shared_ptr<gfx::MeshRegistry>    mesh_registry;
    std::shared_ptr<phys::PhysicsSystem>  physics_system;
    std::shared_ptr<gfx::RenderingSystem> rendering_system;
    
    /// <summary>
    /// Handle keyboard and mouse input within the glfw window--call each frame.
    /// </summary>
    void process_input();

    void init_glfw();

    void init_objects();

    /// <summary>
    /// Initializes GLFW, creates the window, binds the OpenGL context to the window,
    /// initializes GLAD, creates an OpenGL Viewport in the window, and configures
    /// relevant viewport settings for seamless use.
    /// </summary>
    void init();

    /// <summary>
    /// Execute the applications main loop
    /// </summary>
    void main_loop();

    /// <summary>
    /// Clean up application resources after exiting
    /// </summary>
    void cleanup();

  public:
    PhysSimApplication
    (
        std::shared_ptr<gfx::ShaderSystem>    shader_system,
        std::shared_ptr<gfx::MeshRegistry>    mesh_registry,
        std::shared_ptr<phys::PhysicsSystem>  physics_system,
        std::shared_ptr<gfx::RenderingSystem> rendering_system
    );

    /// <summary>
    /// Start & run the application until termination
    /// </summary>
    void run();
};