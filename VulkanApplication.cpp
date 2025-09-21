#include "VulkanApplication.hpp"

static std::vector<char> ReadFile(const std::string& file_name)
{
    std::ifstream file(file_name, std::ios::ate | std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file; already open!");
    }

    std::vector<char> buffer(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
    file.close();
    return buffer;
}

vk::raii::ShaderModule VulkanApplication::CreateShaderModule(const std::vector<char>& bytes) const
{
    vk::ShaderModuleCreateInfo create_info
    {
        .codeSize = bytes.size() * sizeof(char),
        .pCode = reinterpret_cast<const uint32_t*>(bytes.data())
    };

    vk::raii::ShaderModule shader_module{ vkb_device_.device, create_info }; // BLOCKED . CAN'T CONVERT VKB::DEVICE TO RAII EASILY 
    // It was at this moment I gave up on Vulkan. I don't care about graphics programming enough I'm sorry. 
    
}

void VulkanApplication::InitWindow()
{
    glfwInit();
    // Skip creating an OpenGL context
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    // Disable window resizing
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window_ = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan Application", nullptr, nullptr);
}

bool VulkanApplication::InitVulkan()
{
    // Simple bootstrap code taken from:
    // https://github.com/charles-lunarg/vk-bootstrap/blob/main/example/basic_usage.cpp

    vkb::InstanceBuilder instance_builder;
    auto instance_result
    {
        instance_builder
        .set_app_name("Example Vulkan Application")
        .enable_validation_layers()
        .use_default_debug_messenger()
        .build()
    };
        
    if (!instance_result)
    {
        std::cerr << "Failed to create Vulkan instance. Error: " << instance_result.error().message() << std::endl;
        return false;
    }
    vkb_instance_ = instance_result.value();
   
    VkResult glfw_result = glfwCreateWindowSurface(vkb_instance_, window_, nullptr, &surface_);
    if (glfw_result != VK_SUCCESS)
    {
        std::cerr << "Failed to select create window surface. Error: " << std::to_string(glfw_result) << std::endl;
        return false;
    }

    vkb::PhysicalDeviceSelector physical_device_selector{ vkb_instance_ };
    auto physical_device_selector_result = physical_device_selector.set_surface(surface_).select();
    if (!physical_device_selector_result)
    {
        std::cerr << "Failed to select Vulkan Physical Device. Error: " << physical_device_selector_result.error().message() << std::endl;
        if (physical_device_selector_result.error() == vkb::PhysicalDeviceError::no_suitable_device)
        {
            const auto& detailed_reasons = physical_device_selector_result.detailed_failure_reasons();
            if (!detailed_reasons.empty())
            {
                std::cerr << "GPU Selection failure reasons:\n";
                for (const std::string& reason : detailed_reasons)
                {
                    std::cerr << reason << "\n";
                }
            }
        }
        return false;
    }

    vkb::DeviceBuilder device_builder{ physical_device_selector_result.value() };
    // automatically propagate needed data from instance & physical device
    auto device_result = device_builder.build();
    if (!device_result)
    {
        std::cerr << "Failed to create Vulkan device. Error: " << device_result.error().message() << std::endl;
        return false;
    }
    vkb_device_ = device_result.value();

    // Get the graphics queue with a helper function
    auto graphics_queue_result = vkb_device_.get_queue(vkb::QueueType::graphics);
    if (!graphics_queue_result) {
        std::cerr << "Failed to get graphics queue. Error: " << graphics_queue_result.error().message() << std::endl;
        return false;
    }
    graphics_queue_ = graphics_queue_result.value();

    vkb::SwapchainBuilder swapchain_builder{ vkb_device_ };
    auto swapchain_result = swapchain_builder.build();
    if (!swapchain_result)
    {
        std::cout << swapchain_result.error().message() << std::endl;
        return false;
    }
    vkb_swapchain_ = swapchain_result.value();

    // Create the Graphics Pipeline
    auto shader_code = ReadFile("shaders/slang.spv");

    return true;
}

void VulkanApplication::MainLoop()
{
    UUIDv4::UUID my_uuid = phys::PhysicsEnvSingleton::getInstance().AddDynamicObject(phys::ColliderType::CUBE);

    auto last_update_time = std::chrono::duration<double>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    const double tick_rate = 1.0; // in updates/sec
    double accumulator = 0;

    // Application loop
    while (!glfwWindowShouldClose(window_))
    {
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

        glfwPollEvents();
        // Draw calls here.
    }
}

void VulkanApplication::CleanUp()
{
    vkb::destroy_swapchain(vkb_swapchain_);
    vkb::destroy_device(vkb_device_);
    vkb::destroy_surface(vkb_instance_, surface_);
    vkb::destroy_instance(vkb_instance_);
    glfwDestroyWindow(window_);
    glfwTerminate();
}

void VulkanApplication::Run()
{
    InitWindow();
    if (InitVulkan())
    {
        MainLoop();
        CleanUp();
    }


}