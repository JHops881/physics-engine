#include "PhysSimApplication.hpp"

void frame_buffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void PhysSimApplication::process_input() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void PhysSimApplication::init_glfw() {
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

void PhysSimApplication::init_objects() {
    // Having fun with shapes!
    float vertices[] { 
       // positions        // colors
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, //0
       0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, //1
       0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f, //2
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, //3
      -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, //4
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f, //5
      -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f, //6
      -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, //7
    };

    unsigned int indices[] {
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
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);

    core::MeshID        mesh_id  = mesh_registry->add_mesh({ vbo, vao, ebo });
    core::Model         model    = core::Model(mesh_id, { shader_program, 4 } );
    core::ModelID       model_id = model_registry->add_model(model);
    core::PointMassID   pm_id    = physics_system->add_point_mass(glm::vec3(0.0f, 0.0f, 0.0f));
    core::WorldObjectID wo_id    = world->add_object({model_id, pm_id});
}

void PhysSimApplication::init() {
    init_glfw();
    init_objects();
}

void PhysSimApplication::main_loop() {
    // TODO: this needs to be moved out of here.
    core::Camera   camera    = core::Camera(glm::vec3(0.0f, 0.0f, 10.0f));
    core::CameraID camera_id = core::CameraID(1);

    auto last_update_time = std::chrono::high_resolution_clock::now();
    const double tick_rate = 60.0; // in updates/sec
    const double tick_duration = 1.0 / tick_rate; // 1 second / ticks per second = length of a tick
    double accumulator = 0;

    // Application loop
    while (!glfwWindowShouldClose(window)) {
        process_input();

        auto now_time = std::chrono::high_resolution_clock::now();
        double delta_time = std::chrono::duration<double>(now_time - last_update_time).count();
        last_update_time += now_time - last_update_time; // += delta_time but not converted
        accumulator += delta_time;

        while (accumulator > tick_duration) {
            physics_system->step(tick_duration);
            accumulator -= tick_duration;
        }

        // Rendering here ..
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        rendering_engine->render(camera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void PhysSimApplication::cleanup() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

PhysSimApplication::PhysSimApplication(
    std::shared_ptr<core::ShaderSystem>     shader_system,
    std::shared_ptr<core::MeshRegistry>     mesh_registry,
    std::shared_ptr<core::PhysicsSystem>    physics_system,
    std::shared_ptr<core::Renderer> low_lvl_renderer,
    std::shared_ptr<core::ModelRegistry>    model_registry,
    std::shared_ptr<core::World>            world,
    std::shared_ptr<core::RenderingEngine>  rendering_engine)
    : shader_system(std::move(shader_system)),
      mesh_registry(std::move(mesh_registry)),
      physics_system(std::move(physics_system)),
      low_lvl_renderer(std::move(low_lvl_renderer)),
      model_registry(std::move(model_registry)),
      world(std::move(world)),
      rendering_engine(std::move(rendering_engine))
{
}

void PhysSimApplication::run() {
    init();
    main_loop();
    cleanup();
}