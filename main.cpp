#include <iostream>
#include "Physics.h"
#include "Renderer.h"
#include <chrono>
#include <format>

/*
Display the information about the physical object to the stdout.
Namely, the UUID, posiion, and velocity.
*/
void DebugObject(const UUIDv4::UUID& uuid) {
  phys::ObjectInteractor object =
    phys::PhysicsEnvSingleton::getInstance().GetObject(uuid);
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

int main() {
  GLFWwindow* window =
    gfx::SetupWindow(800, 600, "Physics Engine: Simulated View");

  // Having fun with shapes!
  float vertices[] = {
     0.5f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f
  };

  unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
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
    gfx::CreateShaderProgramRoutine(vertex_shader_source,
      fragment_shader_source);

  unsigned int vbo;
  glGenBuffers(1, &vbo);

  unsigned int vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  unsigned int ebo;
  glGenBuffers(1, &ebo);

  std::cout << "initializing.." << std::endl;

  // Make an object
  UUIDv4::UUID my_uuid =
    phys::PhysicsEnvSingleton::getInstance().AddAnimateObject(
      phys::ColliderType::CUBE
    );

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
      phys::PhysicsEnvSingleton::getInstance().Step(tick_rate);

      DebugObject(my_uuid);

      accumulator -= tick_rate;
    }

    gfx::ProcessInput(window);

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