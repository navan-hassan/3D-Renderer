#include <vulkan_context.hpp>

#include <optional>
#include <cstring> 
#include <stdexcept>

void VulkanContext::init() {

    createInstance();

    
    debug_write("Vulkan context successfully initialized");
    //selectPhysicalDevice();
}

void VulkanContext::destroy() {

    debugUtilsMessenger.destroy(instance, pAllocationCallbacks);

    logicalDevice.destroy();

    if (instance != nullptr) {
        vkDestroySurfaceKHR(instance, surface, nullptr);
        vkDestroyInstance(instance, pAllocationCallbacks);
        debug_write("Vulkan instance successfully destroyed");
    }
    debug_write("Vulkan context successfully destroyed");
}

void VulkanContext::createInstance() {

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

    debugUtilsMessenger.init();

    instanceCreateInfo.enabledLayerCount = NUM_VALIDATION_LAYERS;
    instanceCreateInfo.ppEnabledLayerNames = VALIDATION_LAYERS;
    instanceCreateInfo.pNext = &debugUtilsMessenger.createInfo;

    if (vkCreateInstance(&instanceCreateInfo, pAllocationCallbacks, &instance) != VK_SUCCESS) {
        throw std::runtime_error("ERROR: Failed to create instance!");
    }
    debugUtilsMessenger.create(instance, pAllocationCallbacks);
    //deviceSelector.getPhysicalDevices(&instance);
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

void VulkanContext::selectPhysicalDevice() {
    if (deviceSelector.deviceCount == 0 && surface != nullptr) {
        deviceSelector.getPhysicalDevices(&instance, &surface);
    }

    int selection;

    std::cout << "Select a device from the list: " << std::endl;
    deviceSelector.printDeviceInfoToScreen();

    std::cin >> selection;
    std::cin.ignore();
    if (selection < 0 || selection >= deviceSelector.deviceCount) {
        std::cout << "INVALID SELECTION: " <<  selection << std::endl;
        return;
    }

    selectedDevice = deviceSelector.availablePhysicalDevices.at(selection);

    std::cout << "SELECTED DEVICE " << selection << std::endl;
    selectedDevice.printDeviceInfo();
    logicalDevice.initialize(selectedDevice);
}


void VulkanContext::createSurface(GLFWwindow* window) {
    if (window == nullptr) return;

    if (glfwCreateWindowSurface(instance, window, pAllocationCallbacks, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }

    debug_write("SUCCESSFULLY CREATED VULKAN SURFACE");
}