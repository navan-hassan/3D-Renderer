#ifndef VULKAN_CONTEXT_HPP
#define VULKAN_CONTEXT_HPP

#include <GLFW/glfw3.h>
#include <debug_utilities.hpp>
#include <vector>



// Wrapper class to handle interfacing with Vulkan
class VulkanContext {
public:
    void init();
    void onDestroy();
private:

    VkInstance instance = nullptr;
    VkAllocationCallbacks* pAllocationCallbacks = nullptr;
    DebugUtilsMessenger debugUtilsMessenger{};

    void createInstance();
    VkApplicationInfo createApplicationInfo();
    std::vector<const char*> getRequiredInstanceExtensions();
    bool checkRequiredInstanceExtensions(std::vector<const char*> requiredExtensionNames);
};

#endif
