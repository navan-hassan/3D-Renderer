#include <debug_utils_messenger.hpp>
#include <constants.hpp>
#include <logging_utilities.hpp>

VKAPI_ATTR VkBool32 VKAPI_CALL DebugUtilsMessenger::debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {
    std::cerr << "VALIDATION LAYER: " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}


DebugUtilsMessenger::DebugUtilsMessenger() {}

DebugUtilsMessenger::DebugUtilsMessenger(VkInstance instance, const VkAllocationCallbacks* pAllocator, const VkDebugUtilsMessengerCreateInfoEXT& createInfo)
    :instance(instance), pAllocator(pAllocator)
{
    auto createFunction = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, constants::CREATE_DEBUG_UTILS_MESSENGER_FUNC);

    if (createFunction) {
        createFunction(instance, &createInfo, pAllocator, &debugMessenger);
        logging::DEBUG("SUCCESSFULLY CREATED DEBUG UTILS MESSENGER");
    }
}

DebugUtilsMessenger::~DebugUtilsMessenger() {
    destroy();
}

void DebugUtilsMessenger::init() {
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

    createInfo.pfnUserCallback = debugCallback;
}

void DebugUtilsMessenger::create(VkInstance instance, const VkAllocationCallbacks* pAllocator) {
    auto createFunction = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, constants::CREATE_DEBUG_UTILS_MESSENGER_FUNC);

    if (createFunction != nullptr) {
        createFunction(instance, &createInfo, pAllocator, &debugMessenger);
        logging::DEBUG("SUCCESSFULLY CREATED DEBUG UTILS MESSENGER");
    }
}

void DebugUtilsMessenger::destroy() {
    if (destroyed || debugMessenger == VK_NULL_HANDLE) return;

    auto destroyFunction = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, constants::DESTROY_DEBUG_UTILS_MESSENGER_FUNC);

    if (destroyFunction != nullptr) {
        destroyFunction(instance, debugMessenger, pAllocator);
        logging::DEBUG("SUCCESSFULLY DESTROYED DEBUG UTILS MESSENGER");
        destroyed = true;
    }
}

VkDebugUtilsMessengerCreateInfoEXT DebugUtilsMessenger::getDebugUtilsMessengerCreateInfo() {
    VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    return createInfo;
}