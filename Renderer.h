#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace gfx {
  
  /*
OpenGL Viewport exists inside the GLFW Window. Call this function every time
the GLFW window is resized, so that the viewport matches up with it.
*/
  void FrameBufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
  }

  /*
  Handle keyboard and mouse input within the glfw window--call each frame.
  */
  void ProcessInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
    }
  }

  /*
  Create, compile new shader. Returns OpenGL object ID.
  */
  unsigned int CompileShader(const char* shader_source, int shader_type) {
    unsigned int shader_id = glCreateShader(shader_type);
    glShaderSource(shader_id, 1, &shader_source, NULL);
    glCompileShader(shader_id);

    int success;
    char info_log[512];
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

    const char* shader_type_str = "undefined";
    if (shader_type == GL_VERTEX_SHADER) {
      shader_type_str = "vertex";
    }
    else if (shader_type == GL_FRAGMENT_SHADER) {
      shader_type_str = "fragment";
    }

    if (!success) {
      glGetShaderInfoLog(shader_id, 512, NULL, info_log);
      std::cout << "Error, " << shader_type_str <<
        " shader failed to compile.\n" << info_log << std::endl;
    }

    return shader_id;
  }

  unsigned int CreateShaderProgramRoutine(
    const char* vertex_shader_source,
    const char* fragment_shader_source
  ) {

    unsigned int vertex_shader = CompileShader(vertex_shader_source,
      GL_VERTEX_SHADER);

    unsigned int fragment_shader = CompileShader(fragment_shader_source,
      GL_FRAGMENT_SHADER);

    unsigned int shader_program = glCreateProgram();

    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    int success;
    char info_log[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader_program, 512, NULL, info_log);
      std::cout << "Shader program failed to link.\n" << info_log << std::endl;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_program;

  }

  /*
  Initializes GLFW, creates the window, binds the OpenGL context to the window,
  initializes GLAD, creates an OpenGL Viewport in the window, and configures
  relevant viewport settings for seamless use.
  */
  GLFWwindow* SetupWindow(
    unsigned int window_width,
    unsigned int window_height,
    const char* window_title
  ) {
    // Instantiate the GLFW window.
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create GLFW window object.
    GLFWwindow* window = glfwCreateWindow(
      window_width, window_height, window_title, NULL, NULL);
    if (window == NULL) {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
    }

    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cout << "Failed to intialize GLAD" << std::endl;
    }

    glViewport(0, 0, window_width, window_height);
    glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);

    return window;

  }

}