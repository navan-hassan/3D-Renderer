#ifndef DEBUG_UTILITIES_HPP
#define DEBUG_UTILITIES_HPP
#include <iostream>
#include <GLFW/glfw3.h>
#include <constants.hpp>

#ifdef NDEBUG
constexpr auto ENABLE_VALIDATION_LAYERS = false;
inline void debug_write(const char* message) {}

extern const int NUM_VALIDATION_LAYERS = 0;
extern char* VALIDATION_LAYERS[] = nullptr;

#else
constexpr auto ENABLE_VALIDATION_LAYERS = true;

inline void debug_write(const char* message) {
    std::cerr << "DEBUG: " << message << std::endl;
}

constexpr int NUM_VALIDATION_LAYERS = 1;
constexpr char* VALIDATION_LAYERS[NUM_VALIDATION_LAYERS] = {
    "VK_LAYER_KHRONOS_validation"
};

#endif


static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}

struct DebugUtilsMessenger {

    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    VkDebugUtilsMessengerEXT debugMessenger;

    void init() {
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

        createInfo.pfnUserCallback = debugCallback;
    }

    void create(VkInstance instance, const VkAllocationCallbacks* pAllocator) {
        auto createFunction = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, CREATE_DEBUG_UTILS_MESSENGER_FUNC);

        if (createFunction != nullptr) {
            createFunction(instance, &createInfo, pAllocator, &debugMessenger);
            debug_write("Debug utils messenger successfully created");

        }
    }

    void onDestroy(VkInstance instance, const VkAllocationCallbacks* pAllocator) {

        if (!ENABLE_VALIDATION_LAYERS) return;

        auto destroyFunction = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, DESTROY_DEBUG_UTILS_MESSENGER_FUNC);

        if (destroyFunction != nullptr) {
            destroyFunction(instance, debugMessenger, pAllocator);
            debug_write("Debug utils messenger successfully destroyed");
        }
    }
};

#endif