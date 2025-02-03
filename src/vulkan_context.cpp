#include <vulkan_context.hpp>
#include <debug_utilities.hpp>
#include <optional>
#include <stdexcept>

VkInstance VulkanContext::getInstance() const {
    return this->instance;
}

const VkAllocationCallbacks* VulkanContext::getAllocationCallbacks() const {
    return this->pAllocationCallbacks;
}

void VulkanContext::createInstance(const VulkanContextInitOptions& initOptions) {
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

    populateDebugMessengerCreateInfo(debugCreateInfo);

    if (ENABLE_VALIDATION_LAYERS) {
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

    this->debugUtilsMessenger = DebugMessengerFactory::Create(instance, pAllocationCallbacks, debugCreateInfo);
}

VkApplicationInfo VulkanContext::createApplicationInfo() {
    VkApplicationInfo applicationInfo{};

    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pApplicationName = APPLICATION_NAME;
    applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.pEngineName = ENGINE_NAME;
    applicationInfo.apiVersion = VK_API_VERSION_1_0;

    return applicationInfo;
}

std::vector<const char*> VulkanContext::getRequiredInstanceExtensions() {
    uint32_t requiredExtensionCount = 0;
    const char** pRequiredExtensionNames;
    pRequiredExtensionNames = glfwGetRequiredInstanceExtensions(&requiredExtensionCount);

    std::vector<const char*> requiredExtensions(pRequiredExtensionNames, pRequiredExtensionNames + requiredExtensionCount);

    if (ENABLE_VALIDATION_LAYERS) {
        requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return requiredExtensions;
}

bool VulkanContext::checkRequiredInstanceExtensions(std::vector<const char*> requiredExtensionNames) {
    uint32_t availableExtensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensionNames(availableExtensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, availableExtensionNames.data());

    for (const auto& extensionName : requiredExtensionNames) {
        bool isFound = std::find_if(
            availableExtensionNames.begin(),
            availableExtensionNames.end(),
            [extensionName](const VkExtensionProperties& extensionProperties) {
                return std::strcmp(extensionProperties.extensionName, extensionName) == 0;
            }) != availableExtensionNames.end();

        if (!isFound) {
            return false;
        }
    }
    return true;
}
