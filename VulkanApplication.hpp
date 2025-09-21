#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS

#include <VkBootstrap.h>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

#include <GLFW/glfw3.h>
#include "physics.hpp"
#include <chrono>
#include <format>
#include <iostream>
#include <type_traits>
#include <stdexcept>
#include <cstdlib>
#include <fstream>

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

/// <summary>
/// Read in the binary data from a file
/// </summary>
/// <param name="file_name">: filepath of file</param>
/// <returns>Byte array of all file data</returns>
static std::vector<char> ReadFile(const std::string& file_name);

/// <summary>
/// An application running Vulkan in a GLFW window
/// </summary>
class VulkanApplication
{
private:
    GLFWwindow*         window_;
    vkb::Instance       vkb_instance_;
    VkSurfaceKHR        surface_               = VK_NULL_HANDLE;
    vkb::PhysicalDevice vkb_physical_device_;
    vkb::Device         vkb_device_;
    VkQueue             graphics_queue_;
    vkb::Swapchain      vkb_swapchain_;
    
    /// <summary>
    /// Creates a shader module
    /// </summary>
    /// <param name="bytes">byte array of shader code</param>
    /// <returns>Shader module with shader</returns>
    vk::raii::ShaderModule CreateShaderModule(const std::vector<char>& bytes) const;

    /// <summary>
    /// Initializes GLFW window
    /// </summary>
    void InitWindow();

    /// <summary>
    /// Initialize Vulkan
    /// </summary>
    bool InitVulkan();

    /// <summary>
    /// Execute the applications main loop
    /// </summary>
    void MainLoop();

    /// <summary>
    /// Clean up application resources after exiting
    /// </summary>
    void CleanUp();

public:
    /// <summary>
    /// Start & run the application until termination
    /// </summary>
    void Run();

};