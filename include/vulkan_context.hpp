#ifndef VULKAN_CONTEXT_HPP
#define VULKAN_CONTEXT_HPP

#include <GLFW/glfw3.h>
#include <debug_utilities.hpp>
#include <vector>
#include <device_selector.hpp>

// Wrapper class to handle interfacing with Vulkan
class VulkanContext {
public:
    void init();
    void destroy();
    void selectPhysicalDevice();
    void createSurface(GLFWwindow* window);
private:
    DeviceSelector deviceSelector;
    VkInstance instance = nullptr;
    VkAllocationCallbacks* pAllocationCallbacks = nullptr;
    DebugUtilsMessenger debugUtilsMessenger{};
    PhysicalDevice selectedDevice;
    LogicalDevice logicalDevice;
    VkSurfaceKHR surface = VK_NULL_HANDLE;

    void createInstance();
    VkApplicationInfo createApplicationInfo();
    std::vector<const char*> getRequiredInstanceExtensions();
    bool checkRequiredInstanceExtensions(std::vector<const char*> requiredExtensionNames);
};

#endif
