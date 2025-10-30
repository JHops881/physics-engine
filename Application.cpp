#include "Application.hpp"

void frame_buffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void Application::init_glfw() {
    // Instantiate the GLFW window.
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Create GLFW window object.
    window = glfwCreateWindow(WIDTH, HEIGHT, "Scarab Application", NULL, NULL);
    if (window == NULL) {
        cleanup();
        throw std::runtime_error("Failed to create GLFW window");
    }
    // Bind the OpenGL context to the current GLFW window.
    glfwMakeContextCurrent(window);
    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cleanup();
        throw std::runtime_error("Failed to intialize GLAD");
    }
    // Place the OpenGL viewport in the GLFW window
    glViewport(0, 0, WIDTH, HEIGHT);
    // configuring GLFW window settings.
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // No mouse, for first person
    // automatically readjust viewport to window size changes
    glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback); 
}

void Application::init_objects() {
}

void Application::init() {
    init_glfw();
    init_objects();
}

// At some point, we will rip this entry point out--actually, the window management should get ripped out.
void Application::main_loop() {
    // Grabbing interfaces to all of our services that we need.
    auto renderer_3d = locator->get_service<core::IRenderer3D>();
    auto resource_manager = locator->get_service<core::IResourceManager>();
    auto scene_manager = locator->get_service<core::ISceneManager>();
    auto scene_rendering_manager = locator->get_service<core::ISceneRenderingManager>();
    // This is for a cube.
    std::vector<GLfloat> vertex_data = {
        // FRONT
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, //5
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, //0
       -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, //7
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, //5
       -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, //7
       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, //6
        // RIGHT
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, //2
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, //1
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, //0
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, //2
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, //0
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, //5
        // BACK
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, //3
       -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, //4
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, //1
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, //3
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, //1
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, //2
        // LEFT
       -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, //6
       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, //7
       -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, //4
       -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, //6
       -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, //4
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, //3
        // TOP
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, //0
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, //1
       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, //4
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, //0
       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, //4
       -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, //7
        // BOTTOM
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, //2
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, //5
       -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, //6
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, //2
       -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, //6
       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f  //3
    };
    // This is for light.
    std::vector<GLfloat> vertex_data_light = {
        0.5f, -0.5f,  0.5f, //5
        0.5f,  0.5f,  0.5f, //0
       -0.5f,  0.5f,  0.5f, //7
        0.5f, -0.5f,  0.5f, //5
       -0.5f,  0.5f,  0.5f, //7
       -0.5f, -0.5f,  0.5f, //6

        0.5f, -0.5f, -0.5f, //2
        0.5f,  0.5f, -0.5f, //1
        0.5f,  0.5f,  0.5f, //0
        0.5f, -0.5f, -0.5f, //2
        0.5f,  0.5f,  0.5f, //0
        0.5f, -0.5f,  0.5f, //5

       -0.5f, -0.5f, -0.5f, //3
       -0.5f,  0.5f, -0.5f, //4
        0.5f,  0.5f, -0.5f, //1
       -0.5f, -0.5f, -0.5f, //3
        0.5f,  0.5f, -0.5f, //1
        0.5f, -0.5f, -0.5f, //2

       -0.5f, -0.5f,  0.5f, //6
       -0.5f,  0.5f,  0.5f, //7
       -0.5f,  0.5f, -0.5f, //4
       -0.5f, -0.5f,  0.5f, //6
       -0.5f,  0.5f, -0.5f, //4
       -0.5f, -0.5f, -0.5f, //3

        0.5f,  0.5f,  0.5f, //0
        0.5f,  0.5f, -0.5f, //1
       -0.5f,  0.5f, -0.5f, //4
        0.5f,  0.5f,  0.5f, //0
       -0.5f,  0.5f, -0.5f, //4
       -0.5f,  0.5f,  0.5f, //7

        0.5f, -0.5f, -0.5f, //2
        0.5f, -0.5f,  0.5f, //5
       -0.5f, -0.5f,  0.5f, //6
        0.5f, -0.5f, -0.5f, //2
       -0.5f, -0.5f,  0.5f, //6
       -0.5f, -0.5f, -0.5f, //3
    };
    // cube shader.
    const char* illuminated_vertex = "shaders/illuminated.vert";
    const char* illuminated_fragment = "shaders/illuminated.frag";
    GLuint cube_shader = renderer_3d->new_shader_program(illuminated_vertex, illuminated_fragment);
    // light shader
    const char* illuminator_vertex = "shaders/illuminator.vert";
    const char* illuminator_fragment = "shaders/illuminator.frag";
    GLuint light_shader = renderer_3d->new_shader_program(illuminator_vertex, illuminator_fragment);
    // cube VAO for regular-degular cubes.
    GLuint cube_vbo = renderer_3d->new_VBO(vertex_data);
    GLuint cube_vao = renderer_3d->new_VAO(cube_vbo, { 3, 3 } );
    // unique VAO for the light.
    GLuint light_vbo = renderer_3d->new_VBO(vertex_data_light);
    GLuint light_vao = renderer_3d->new_VAO(light_vbo, { 3 } );
    // skybox texture - load into memory.
    std::vector<std::string> skybox_cubemap_faces = {
        "assets/px.jpg",
        "assets/nx.jpg",
        "assets/py.jpg",
        "assets/ny.jpg",
        "assets/pz.jpg",
        "assets/nz.jpg"
    };
    GLuint skybox_cubemap = resource_manager->load_cubemap(skybox_cubemap_faces);
    // This seems like a dangerous pattern.
    scene_manager->set_skybox(skybox_cubemap);
    scene_rendering_manager->init_skybox_geometry();
    // Camera and User Input Interface (Player Controller (needs to change) ) for movement.
    core::Camera camera = core::Camera(glm::vec3(2.0f, 0.0f, 6.0f), glm::vec3(0.0f, 0.0f, -1.0f));
    core::PlayerController pc = core::PlayerController(window);
    // setup stuff
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    // Loop below ---------------------------------------------------

    auto last_update_time = std::chrono::high_resolution_clock::now();
    const double tick_rate = 60.0; // in updates/sec
    const double tick_duration = 1.0 / tick_rate; // 1 second / ticks per second = length of a tick
    double accumulator = 0;

    // Application loop
    while (!glfwWindowShouldClose(window)) {
        auto now_time = std::chrono::high_resolution_clock::now();
        double delta_time = std::chrono::duration<double>(now_time - last_update_time).count();
        last_update_time += now_time - last_update_time; // += delta_time but not converted
        accumulator += delta_time;

        pc.process_input(camera, delta_time);

        while (accumulator > tick_duration) {
            // physics_system->step(tick_duration);
            accumulator -= tick_duration;
        }
        // Clean wash for each frame.
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // draw a light.
        glm::vec3 light_position = glm::vec3(0.0f, 0.0f, 0.0f);
        renderer_3d->draw_illuminator(light_vao, light_shader, light_position, camera, 36);
        // box
        renderer_3d->draw_illuminated(
            cube_vao, cube_shader, glm::vec3(2.0f, 1.5f, 1.0f), camera, 36,
            glm::vec3(1.0f, 0.5f, 0.31f), glm::vec3(1.0f), light_position);
        renderer_3d->draw_illuminated(
            cube_vao, cube_shader, glm::vec3(3.0f), camera, 36,
            glm::vec3(1.0f, 0.5f, 0.31f), glm::vec3(1.0f), light_position);
        renderer_3d->draw_illuminated(
            cube_vao, cube_shader, glm::vec3(0.0f, -1.7f, -3.1f), camera, 36,
            glm::vec3(1.0f, 0.5f, 0.31f), glm::vec3(1.0f), light_position);
        renderer_3d->draw_illuminated(
            cube_vao, cube_shader, glm::vec3(-3.0f, 0.0f, 1.0f), camera, 36,
            glm::vec3(1.0f, 0.5f, 0.31f), glm::vec3(1.0f), light_position);
        renderer_3d->draw_illuminated(
            cube_vao, cube_shader, glm::vec3(0.0f, -1.0f, 4.0f), camera, 36,
            glm::vec3(1.0f, 0.5f, 0.31f), glm::vec3(1.0f), light_position);
        // Draw the skybox, always last
        // https://freestylized.com/all-skybox/ get some good ones here.
        scene_rendering_manager->render_skybox(camera); 

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Application::cleanup() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

Application::Application(std::shared_ptr<core::ServiceLocator> locator)
    : locator(locator)
{
}

void Application::run() {
    init();
    main_loop();
    cleanup();
}