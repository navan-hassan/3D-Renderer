#include <vulkan_context.hpp>
#include <logging_utilities.hpp>
#include <constants.hpp>

#include <optional>
#include <stdexcept>

VulkanContext::VulkanContext(const VulkanContextInitOptions& initOptions, std::shared_ptr<GLFWContext> glfwContext)
    : glfwContext(glfwContext) {
    createInstance(initOptions);
    logging::DEBUG("SUCCESSFULLY CREATED VULKAN CONTEXT");
}

VulkanContext::~VulkanContext() {
    if (debugUtilsMessenger) {
        debugUtilsMessenger->destroy();
    }

    if (instance) {
        vkDestroyInstance(instance, pAllocationCallbacks);
    }
    logging::DEBUG("SUCCESSFULLY DESTROYED VULKAN CONTEXT");
}

VkInstance VulkanContext::getInstance() const {
    return instance;
}

const VkAllocationCallbacks* VulkanContext::getAllocationCallbacks() const {
    return pAllocationCallbacks;
}

void VulkanContext::createInstance(const VulkanContextInitOptions& initOptions) {
    enableValidationLayers = initOptions.enableValidationLayers;

    VkApplicationInfo applicationInfo = createApplicationInfo();

    std::vector<const char*> extensionNames = getRequiredInstanceExtensions();
    if (!checkRequiredInstanceExtensions(extensionNames)) {
        throw std::runtime_error("ERROR: Required instance extensions were not available!");
    }

    VkInstanceCreateInfo instanceCreateInfo{};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo = &applicationInfo;
    instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensionNames.size());
    instanceCreateInfo.ppEnabledExtensionNames = extensionNames.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};

    if (enableValidationLayers) {
        debugCreateInfo = DebugUtilsMessenger::getDebugUtilsMessengerCreateInfo();
        instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(initOptions.validationLayers.size());
        instanceCreateInfo.ppEnabledLayerNames = initOptions.validationLayers.data();
        instanceCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }
    else {
        instanceCreateInfo.enabledLayerCount = 0;
        instanceCreateInfo.pNext = nullptr;
    }

    if (vkCreateInstance(&instanceCreateInfo, pAllocationCallbacks, &instance) != VK_SUCCESS) {
        throw std::runtime_error("ERROR: Failed to create instance!");
    }

    if (enableValidationLayers) {
        debugUtilsMessenger = std::make_unique<DebugUtilsMessenger>(instance, pAllocationCallbacks, debugCreateInfo);
    }
}

VkApplicationInfo VulkanContext::createApplicationInfo() {
    VkApplicationInfo applicationInfo{};

    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pApplicationName = constants::APPLICATION_NAME;
    applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.pEngineName = constants::ENGINE_NAME;
    applicationInfo.apiVersion = VK_API_VERSION_1_0;

    return applicationInfo;
}

std::vector<const char*> VulkanContext::getRequiredInstanceExtensions() const {
    std::vector<const char*> requiredExtensions = glfwContext->getRequiredInstanceExtensions();

    if (enableValidationLayers) {
        requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return requiredExtensions;
}

bool VulkanContext::checkRequiredInstanceExtensions(const std::vector<const char*>& requiredExtensionNames) const {
    uint32_t availableExtensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensionNames(availableExtensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, availableExtensionNames.data());

    for (const auto& requiredExtensionName : requiredExtensionNames) {
        bool isFound = std::find_if(
            availableExtensionNames.begin(),
            availableExtensionNames.end(),
            [requiredExtensionName](const VkExtensionProperties& extensionProperties) {
                return std::strcmp(extensionProperties.extensionName, requiredExtensionName) == 0;
            }) != availableExtensionNames.end();

        if (!isFound) {
            return false;
        }
    }
    return true;
}