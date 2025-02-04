#ifndef VULKAN_CONTEXT_HPP
#define VULKAN_CONTEXT_HPP

#include <memory>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <debug_utilities.hpp>

struct VulkanContextInitOptions {
    bool enableValidationLayers = false;
    std::vector<const char*> validationLayers;
};

// Wrapper class to handle interfacing with Vulkan
class VulkanContext {
public:
    VulkanContext(const VulkanContextInitOptions& initOptions) {
        glfwInit();
        createInstance(initOptions);
        debug_write("Vulkan context successfully created");
    }

    ~VulkanContext() {
        if (this->debugUtilsMessenger != VK_NULL_HANDLE) {
            this->debugUtilsMessenger->~DebugUtilsMessenger();
        }

        if (instance != VK_NULL_HANDLE) {
            vkDestroyInstance(instance, pAllocationCallbacks);
            debug_write("Vulkan instance successfully destroyed");
        }
        glfwTerminate();
        debug_write("Vulkan context successfully destroyed");
    }

    VkInstance getInstance() const;
    const VkAllocationCallbacks* getAllocationCallbacks() const;
private:
    bool enableValidationLayers = false;
    VkInstance instance = VK_NULL_HANDLE;
    VkAllocationCallbacks* pAllocationCallbacks = VK_NULL_HANDLE;
    std::shared_ptr<DebugUtilsMessenger> debugUtilsMessenger = VK_NULL_HANDLE;
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    void createInstance(const VulkanContextInitOptions& initOptions);
    VkApplicationInfo createApplicationInfo();
    std::vector<const char*> getRequiredInstanceExtensions();
    bool checkRequiredInstanceExtensions(std::vector<const char*> requiredExtensionNames);
};

#endif
