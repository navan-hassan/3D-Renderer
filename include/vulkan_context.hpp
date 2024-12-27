#ifndef VULKAN_CONTEXT_HPP
#define VULKAN_CONTEXT_HPP

#include <GLFW/glfw3.h>
#include <vector>

#ifdef NDEBUG
constexpr auto ENABLE_VALIDATION_LAYERS = false;
#else
constexpr auto ENABLE_VALIDATION_LAYERS = true;
#endif

// Wrapper class to handle interfacing with Vulkan
class VulkanContext {
public:
    void init();
    void onDestroy();
private:

    VkInstance instance = nullptr;
    VkAllocationCallbacks* pAllocationCallbacks = nullptr;

    void createInstance();
    VkApplicationInfo createApplicationInfo();
    std::vector<const char*> getRequiredInstanceExtensions();
    bool checkRequiredInstanceExtensions(std::vector<const char*> requiredExtensionNames);
};

#endif
