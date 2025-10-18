#pragma once
#include "MeshRegistry.hpp"
#include "PhysicsSystem.hpp"
#include "LowLevelRenderer.hpp"
#include "ShaderSystem.hpp"
#include "ModelRegistry.hpp"
#include "World.hpp"
#include "RenderingEngine.hpp"

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
    GLFWwindow*                             window;
    std::shared_ptr<core::ShaderSystem>     shader_system;
    std::shared_ptr<core::MeshRegistry>     mesh_registry;
    std::shared_ptr<core::PhysicsSystem>    physics_system;
    std::shared_ptr<core::LowLevelRenderer> low_lvl_renderer;
    std::shared_ptr<core::ModelRegistry>    model_registry;
    std::shared_ptr<core::World>            world;
    std::shared_ptr<core::RenderingEngine>  rendering_engine;

    
    /// <summary>
    /// Handle keyboard and mouse input within the glfw window--call each frame.
    /// </summary>
    void process_input();

    /// <summary>
    /// Initialize GLFW.
    /// </summary>
    void init_glfw();

    /// <summary>
    /// Initialize simulated objects.
    /// </summary>
    void init_objects();

    /// <summary>
    /// Initializes GLFW, creates the window, binds the OpenGL context to the window,
    /// initializes GLAD, creates an OpenGL Viewport in the window, and configures
    /// relevant viewport settings for seamless use.
    /// </summary>
    void init();

    /// <summary>
    /// Execute the application's main loop while it is running.
    /// </summary>
    void main_loop();

    /// <summary>
    /// Clean up application resources after exiting.
    /// </summary>
    void cleanup();

  public:
    /// 
    /// Create a new Physics Simulation Application.
    // fix documentation
    PhysSimApplication
    (
        std::shared_ptr<core::ShaderSystem>     shader_system,
        std::shared_ptr<core::MeshRegistry>     mesh_registry,
        std::shared_ptr<core::PhysicsSystem>    physics_system,
        std::shared_ptr<core::LowLevelRenderer> low_lvl_renderer,
        std::shared_ptr<core::ModelRegistry>    model_registry,
        std::shared_ptr<core::World>            world,
        std::shared_ptr<core::RenderingEngine>  rendering_engine
    );

    /// <summary>
    /// Start & run the application until termination.
    /// </summary>
    void run();
};