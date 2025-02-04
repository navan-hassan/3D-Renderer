#include <debug_utilities.hpp>

DebugUtilsMessenger::DebugUtilsMessenger() {}

DebugUtilsMessenger::DebugUtilsMessenger(VkInstance instance, const VkAllocationCallbacks* pAllocator, const VkDebugUtilsMessengerCreateInfoEXT& createInfo)
    :instance(instance), pAllocator(pAllocator)
{
    auto createFunction = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, CREATE_DEBUG_UTILS_MESSENGER_FUNC);

    if (createFunction != nullptr) {
        createFunction(instance, &createInfo, pAllocator, &debugMessenger);
        debug_write("Debug utils messenger successfully created");
    }
}

DebugUtilsMessenger::~DebugUtilsMessenger() {
    if (destroyed || debugMessenger == VK_NULL_HANDLE) return;

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
    auto createFunction = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, CREATE_DEBUG_UTILS_MESSENGER_FUNC);

    if (createFunction != nullptr) {
        createFunction(instance, &createInfo, pAllocator, &debugMessenger);
        debug_write("Debug utils messenger successfully created");
    }
}

void DebugUtilsMessenger::destroy() {
    if (destroyed || debugMessenger == VK_NULL_HANDLE) return;

    auto destroyFunction = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, DESTROY_DEBUG_UTILS_MESSENGER_FUNC);

    if (destroyFunction != nullptr) {
        destroyFunction(instance, debugMessenger, pAllocator);
        debug_write("Debug utils messenger successfully destroyed");
        destroyed = true;
    }
}

std::shared_ptr<DebugUtilsMessenger> DebugMessengerFactory::Create(VkInstance parentInstance, const VkAllocationCallbacks* pAllocator, const VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    std::shared_ptr<DebugUtilsMessenger> debugUtilsMessenger = std::shared_ptr<DebugUtilsMessenger>(new DebugUtilsMessenger(parentInstance, pAllocator, createInfo));
    return debugUtilsMessenger;
}
