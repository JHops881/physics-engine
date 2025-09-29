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
       0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
       0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
      -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
      -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f
    };

    unsigned int indices[]
    {
      0, 1, 3,
      1, 2, 3
    };

    const char* vertex_shader_source = "shaders/shader.vert";
    const char* fragment_shader_source = "shaders/shader.frag";

    uint32_t shader_program = create_shader_program(vertex_shader_source, fragment_shader_source);

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

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    uint32_t entity = entity_manager.new_entity();
    entity_manager.attach_collider(entity, ecs::ColliderType::CUBE);
    entity_manager.attach_position(entity, ecs::Position{ glm::vec3(0.0f) });
    entity_manager.attach_rigid_body(entity, {
    glm::vec3(0.0f),
    glm::vec3(0.0f),
    1.0f
    });
    entity_manager.attach_mesh(entity, {vbo, vao, ebo, shader_program});
}

void PhysSimApplication::init()
{
    init_glfw();
    init_objects();
}

void PhysSimApplication::main_loop()
{
    auto last_update_time = std::chrono::duration<double>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    const double tick_rate = 1.0; // in updates/sec
    double accumulator = 0;

    // Application loop
    while (!glfwWindowShouldClose(window))
    {
        process_input();

        double now_time = std::chrono::duration<double>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        double delta_time = now_time - last_update_time;
        last_update_time += delta_time;
        accumulator += delta_time;

        while (accumulator > tick_rate)
        {
            entity_manager.physics_step(tick_rate);

            entity_manager.debug_objects();

            accumulator -= tick_rate;
        }

        // Rendering here ..
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        entity_manager.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void PhysSimApplication::cleanup()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void PhysSimApplication::run()
{
    init();
    main_loop();
    cleanup();
}