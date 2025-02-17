#ifndef DEBUG_UTILITIES_HPP
#define DEBUG_UTILITIES_HPP
#include <iostream>
#include <glfw_context.hpp>

class DebugUtilsMessenger {
public:
    DebugUtilsMessenger();
    DebugUtilsMessenger(VkInstance instance, 
        const VkAllocationCallbacks* pAllocator, 
        const VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    ~DebugUtilsMessenger();
    void init();
    void create(VkInstance instance, const VkAllocationCallbacks* pAllocator);
    void destroy();
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    static VkDebugUtilsMessengerCreateInfoEXT getDebugUtilsMessengerCreateInfo();
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData);
private:
    bool destroyed = false;
    VkInstance instance = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
    const VkAllocationCallbacks* pAllocator = VK_NULL_HANDLE;
};

#endif