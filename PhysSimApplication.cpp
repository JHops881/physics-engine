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
}

void PhysSimApplication::init() {
    init_glfw();
    init_objects();
}

void PhysSimApplication::main_loop() {

    auto physics_system = locator->get_service<core::IPhysicsSystem>();
    auto renderer_3d = locator->get_service<core::IRenderer3D>();

    // TODO: All of this needs to leave
    std::vector<GLfloat> vertex_data {
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

    std::vector<GLushort> indices {
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

    const char* vertex_shader_filepath = "shaders/shader.vert";
    const char* fragment_shader_filepath = "shaders/shader.frag";
    GLuint shader = renderer_3d->new_shader_program(vertex_shader_filepath, fragment_shader_filepath);

    GLuint VBO = renderer_3d->new_VBO(vertex_data);
    GLuint EBO = renderer_3d->new_EBO(indices);
    GLuint VAO = renderer_3d->new_VAO(VBO, EBO, 2, {3, 3});

    core::Camera camera = core::Camera(glm::vec3(0.0f, 0.0f, 10.0f));

    core::PointMassID pm_id = physics_system->add_point_mass(glm::vec3(0.0f, 0.0f, 0.0f));

    // setup stuff
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);

    // Loop below ---------------------------------------------------

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

        core::PointMass& pm = physics_system->get_point_mass(pm_id);
        renderer_3d->draw_indexed_geometry(VAO, shader, 1, 36, pm.position, camera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void PhysSimApplication::cleanup() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

PhysSimApplication::PhysSimApplication(std::shared_ptr<core::ServiceLocator> locator) : locator(locator) {}

void PhysSimApplication::run() {
    init();
    main_loop();
    cleanup();
}