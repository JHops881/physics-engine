#include <iostream>
#include "Physics.h"
#include <chrono>
#include <format>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

/*
Display the information about the physical object to the stdout. 
Namely, the UUID, posiion, and velocity.
*/
void DebugObject(const UUIDv4::UUID& uuid) {
  phys::Object& object =
    phys::PhysicsWorldSingleton::getInstance().GetObject(uuid);
  const glm::vec3& position = object.GetPosition();
  const glm::vec3& velocity = object.GetVelocity();

  std::cout << "Object UUID: " << uuid << std::endl;

  std::string message1 = std::format("Position: ( x: {}, y: {}, z: {})",
    position.x, position.y, position.z);

  std::cout << message1 << std::endl;

  std::string message2 = std::format("Velocity: x: {}m/s, y: {}m/s, z: {}m/s",
    velocity.x, velocity.y, velocity.z);

  std::cout << message2 << std::endl << std::endl;
}

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

  const char* shader_type_str;
  if (shader_type == GL_VERTEX_SHADER) {
    shader_type_str = "vertex";
  }
  else if (shader_type == GL_FRAGMENT_SHADER){
     shader_type_str = "fragment";
  }
  
  if (!success) {
    glGetShaderInfoLog(shader_id, 512, NULL, info_log);
    std::cout << "Error, " << shader_type << " shader failed to compile.\n"
      << info_log << std::endl;
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

int main() {

  // Instantiate the GLFW window.
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create GLFW window object.
  unsigned int window_width  = 800, window_height = 600;
  const char* window_title = "Physics Engine: Simulated View";
  GLFWwindow* window = glfwCreateWindow(
    window_width, window_height, window_title, NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // Initialize GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to intialize GLAD" << std::endl;
    return -1;
  }

  glViewport(0, 0, window_width, window_height);
  glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
    
  // Having fun with shapes!
  float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f,
  };

  const char* vertex_shader_source = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

  const char* fragment_shader_source = 
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

  unsigned int shader_program =
    CreateShaderProgramRoutine(vertex_shader_source, fragment_shader_source);

    unsigned int vbo;
  glGenBuffers(1, &vbo);

  unsigned int vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);


  std::cout << "initializing.." << std::endl;
  // Make an object
  phys::Object my_object;
  UUIDv4::UUID my_uuid =
    phys::PhysicsWorldSingleton::getInstance().AddObject(my_object);


  auto last_update_time = std::chrono::duration<double>(
    std::chrono::high_resolution_clock::now().time_since_epoch()
  ).count();
  const double tick_rate = 1.0; // in updates/sec
  double accumulator = 0;

  // Application loop
  while (!glfwWindowShouldClose(window)) {
    
    double now_time = std::chrono::duration<double>(
      std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count();
    double delta_time = now_time - last_update_time;
    last_update_time += delta_time;
    accumulator += delta_time;

    while (accumulator > tick_rate) {
      phys::PhysicsWorldSingleton::getInstance().Step(tick_rate);

      DebugObject(my_uuid);

      accumulator -= tick_rate;
    }

    ProcessInput(window);

    // Rendering here .. 
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
    
  }

  // Clean Up.
  glfwTerminate();
  return 0;
}