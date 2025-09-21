#include "OpenGLApplication.hpp"

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

std::string ReadFile(const char* filepath)
{
    std::ifstream file(filepath);
    std::string file_contents{ std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
    return file_contents;
}

void OpenGLApplication::ProcessInput()
{
    if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window_, true);
    }
}

unsigned int OpenGLApplication::CompileShader(const char* shader_str, int shader_type)
{
    unsigned int shader_id = glCreateShader(shader_type);
    glShaderSource(shader_id, 1, &shader_str, NULL);
    glCompileShader(shader_id);

    int success;
    char info_log[512];
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

    const char* shader_type_str = "undefined";
    if (shader_type == GL_VERTEX_SHADER)
    {
        shader_type_str = "vertex";
    }
    else if (shader_type == GL_FRAGMENT_SHADER)
    {
        shader_type_str = "fragment";
    }

    if (!success)
    {
        glGetShaderInfoLog(shader_id, 512, NULL, info_log);
        std::string error = std::format("Error, {} shader failed to compile\n{}", shader_type_str, info_log);
        throw std::runtime_error(error);
    }

    return shader_id;
}

unsigned int OpenGLApplication::CreateShaderProgramRoutine(const char* vertex_shader_source, const char* fragment_shader_source)
{
    std::string  vertex_shader_str   = ReadFile(vertex_shader_source);
    std::string  fragment_shader_str = ReadFile(fragment_shader_source);
    unsigned int vertex_shader       = CompileShader(vertex_shader_str.c_str(), GL_VERTEX_SHADER);
    unsigned int fragment_shader     = CompileShader(fragment_shader_str.c_str(), GL_FRAGMENT_SHADER);

    unsigned int shader_program = glCreateProgram();

    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    int success;
    char info_log[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        std::string error = std::format("Shader program failed to link.\n{}", info_log);
        throw std::runtime_error(error);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
}

void OpenGLApplication::Init()
{
    // Instantiate the GLFW window.
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create GLFW window object.
    window_ = glfwCreateWindow(
        WIDTH, HEIGHT, "OpenGL Application", NULL, NULL);
    if (window_ == NULL) {
        CleanUp();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window_);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        CleanUp();
        throw std::runtime_error("Failed to intialize GLAD");
    }

    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window_, FrameBufferSizeCallback);
}

void OpenGLApplication::MainLoop()
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

    unsigned int shader_program = CreateShaderProgramRoutine(vertex_shader_source, fragment_shader_source);

    unsigned int vbo;
    glGenBuffers(1, &vbo);

    unsigned int vao;
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

    unsigned int ebo;
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    UUIDv4::UUID my_uuid = phys::PhysicsEnvSingleton::getInstance().AddDynamicObject(phys::ColliderType::CUBE);

    auto last_update_time = std::chrono::duration<double>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    const double tick_rate = 1.0; // in updates/sec
    double accumulator = 0;

    // Application loop
    while (!glfwWindowShouldClose(window_))
    {
        ProcessInput();

        double now_time = std::chrono::duration<double>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        double delta_time = now_time - last_update_time;
        last_update_time += delta_time;
        accumulator += delta_time;

        while (accumulator > tick_rate)
        {
            phys::PhysicsEnvSingleton::getInstance().Step(tick_rate);

            phys::PhysicsEnvSingleton::getInstance().DebugObject(my_uuid);

            accumulator -= tick_rate;
        }

        // Rendering here ..
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window_);
        glfwPollEvents();
    }
}

void OpenGLApplication::CleanUp()
{
    glfwDestroyWindow(window_);
    glfwTerminate();
}

void OpenGLApplication::Run()
{
    Init();
    MainLoop();
    CleanUp();
}