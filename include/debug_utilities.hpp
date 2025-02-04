#ifndef DEBUG_UTILITIES_HPP
#define DEBUG_UTILITIES_HPP
#include <iostream>
#include <GLFW/glfw3.h>
#include <constants.hpp>

inline void debug_write(const char* message) {
#ifndef NDEBUG
    std::cerr << "DEBUG: " << message << std::endl;
#endif
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) 
{
    std::cerr << "VALIDATION LAYER: " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}

static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
}

class DebugUtilsMessenger {
public:
    DebugUtilsMessenger();
    DebugUtilsMessenger(VkInstance instance, const VkAllocationCallbacks* pAllocator, const VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    ~DebugUtilsMessenger();
    void init();
    void create(VkInstance instance, const VkAllocationCallbacks* pAllocator);
    void destroy();
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
private:
    bool destroyed = false;
    VkInstance instance = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
    const VkAllocationCallbacks* pAllocator = VK_NULL_HANDLE;
};

struct DebugMessengerFactory {
    static std::shared_ptr<DebugUtilsMessenger> Create(VkInstance parentInstance, const VkAllocationCallbacks* pAllocator, const VkDebugUtilsMessengerCreateInfoEXT& createInfo);
};



#endif