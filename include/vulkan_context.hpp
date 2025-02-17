#ifndef VULKAN_CONTEXT_HPP
#define VULKAN_CONTEXT_HPP

#include <memory>
#include <vector>

#include <glfw_context.hpp>
#include <debug_utils_messenger.hpp>

struct VulkanContextInitOptions {
    bool enableValidationLayers = false;
    std::vector<const char*> validationLayers;
};

// Wrapper class to handle interfacing with Vulkan
class VulkanContext {
public:
    VulkanContext(const VulkanContextInitOptions& initOptions, std::shared_ptr<GLFWContext> glfwContext);
    ~VulkanContext();
    VkInstance getInstance() const;
    const VkAllocationCallbacks* getAllocationCallbacks() const;
private:
    bool enableValidationLayers = false;
    VkInstance instance = VK_NULL_HANDLE;
    VkAllocationCallbacks* pAllocationCallbacks = VK_NULL_HANDLE;
    std::unique_ptr<DebugUtilsMessenger> debugUtilsMessenger = VK_NULL_HANDLE;
    std::shared_ptr<GLFWContext> glfwContext;
    void createInstance(const VulkanContextInitOptions& initOptions);
    static VkApplicationInfo createApplicationInfo();
    std::vector<const char*> getRequiredInstanceExtensions() const;
    bool checkRequiredInstanceExtensions(const std::vector<const char*>& requiredExtensionNames) const;

};

#endif
