#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "physics.hpp"

#include <chrono>
#include <format>
#include <iostream>
#include <type_traits>
#include <stdexcept>
#include <cstdlib>
#include <fstream>

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

/// <summary>
/// OpenGL Viewport exists inside the GLFW Window. Call this function every time
/// the GLFW window is resized, so that the viewport matches up with it.
/// </summary>
/// <param name="window">: pointer to active GLFW window with OpenGL context</param>
/// <param name="width">: (automatically passed in) new window width</param>
/// <param name="height">: (automatically passed in) new window width</param>
static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);

std::string ReadFile(const char* filepath);

/// <summary>
/// An application running Vulkan in a GLFW window
/// </summary>
class OpenGLApplication
{
  private:
    GLFWwindow* window_;
    
    /// <summary>
    /// Handle keyboard and mouse input within the glfw window--call each frame.
    /// </summary>
    void ProcessInput();

    unsigned int CompileShader(const char* shader_source, int shader_type);

    unsigned int CreateShaderProgramRoutine(const char* vertex_shader_source, const char* fragment_shader_source);

    /// <summary>
    /// Initializes GLFW, creates the window, binds the OpenGL context to the window,
    /// initializes GLAD, creates an OpenGL Viewport in the window, and configures
    /// relevant viewport settings for seamless use.
    /// </summary>
    void Init();

    /// <summary>
    /// Execute the applications main loop
    /// </summary>
    void MainLoop();

    /// <summary>
    /// Clean up application resources after exiting
    /// </summary>
    void CleanUp();

  public:
    /// <summary>
    /// Start & run the application until termination
    /// </summary>
    void Run();

};