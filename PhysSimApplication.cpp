#include "PhysSimApplication.hpp"

void frame_buffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void PhysSimApplication::process_input()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void PhysSimApplication::init_glfw()
{
    // Instantiate the GLFW window.
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create GLFW window object.
    window = glfwCreateWindow(
        WIDTH, HEIGHT, "OpenGL Application", NULL, NULL);
    if (window == NULL) {
        cleanup();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cleanup();
        throw std::runtime_error("Failed to intialize GLAD");
    }

    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);
}

void PhysSimApplication::init_objects()
{
    // Having fun with shapes!
    float vertices[]
    {  // positions        // colors
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, //0
       0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, //1
       0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f, //2
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, //3
      -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, //4
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f, //5
      -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f, //6
      -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, //7
    };

    unsigned int indices[]
    {
      3, 2, 1,
      1, 4, 3,

      6, 5, 0,
      0, 7, 6,

      7, 4, 3,
      3, 6, 7,

      0, 1, 2,
      2, 5, 0,

      3, 2, 5,
      5, 6, 3,

      4, 1, 0,
      0, 7, 4
    };

    const char* vertex_shader_source = "shaders/shader.vert";
    const char* fragment_shader_source = "shaders/shader.frag";

    uint32_t shader_program = shader_system->create_shader_program(vertex_shader_source, fragment_shader_source);

    uint32_t vbo;
    glGenBuffers(1, &vbo);

    uint32_t vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // position attr
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attr
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    uint32_t ebo;
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // setup stuff
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);

    phys::DynamicID   phys_id = physics_system->add_dynamic(glm::vec3(0.0f, 6.0f, -10.5f), glm::vec3(0.0f), glm::vec3(0.0f), 1.0f);
    gfx::MeshID       mesh_id = mesh_registry->add_mesh({ vbo, vao, ebo, shader_program });
    gfx::RenderableID rend_id = rendering_system->new_renderable({mesh_id, phys_id});
}

void PhysSimApplication::init()
{
    init_glfw();
    init_objects();
}

void PhysSimApplication::main_loop()
{
    auto last_update_time = std::chrono::duration<double>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    const double tick_rate = 60.0; // in updates/sec
    const double tick_duration = 1.0 / tick_rate; // 1 second / ticks per second = length of a tick
    double accumulator = 0;

    // Application loop
    while (!glfwWindowShouldClose(window))
    {
        process_input();

        double now_time = std::chrono::duration<double>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        double delta_time = now_time - last_update_time;
        last_update_time += delta_time;
        accumulator += delta_time;

        while (accumulator > tick_duration)
        {
            physics_system->step(tick_duration);

            physics_system->debug_objects();

            accumulator -= tick_duration;
        }

        // Rendering here ..
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        rendering_system->render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void PhysSimApplication::cleanup()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

PhysSimApplication::PhysSimApplication
(
    std::shared_ptr<gfx::ShaderSystem>    shader_system,
    std::shared_ptr<gfx::MeshRegistry>    mesh_registry,
    std::shared_ptr<phys::PhysicsSystem>  physics_system,
    std::shared_ptr<gfx::RenderingSystem> rendering_system
)
    :
    shader_system(std::move(shader_system)),
    mesh_registry(std::move(mesh_registry)),
    physics_system(std::move(physics_system)),
    rendering_system(std::move(rendering_system))
{
}

void PhysSimApplication::run()
{
    init();
    main_loop();
    cleanup();
}