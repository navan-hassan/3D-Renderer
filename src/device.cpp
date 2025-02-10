#include <device.hpp>

#include <set>

bool QueueFamilyIndex::isComplete() const {
    return graphics.has_value() && presentation.has_value();
}

PhysicalDeviceSelectionInfo::~PhysicalDeviceSelectionInfo() {
    surface = VK_NULL_HANDLE;
    requestedDeviceExtensions.clear();
}

DeviceInterfaceCreationInfo::~DeviceInterfaceCreationInfo() {
    physicalDevice = VK_NULL_HANDLE;
    requestedDeviceExtensions.clear();
    queueFamilyIndex.graphics.reset();
    queueFamilyIndex.presentation.reset();
}

LogicalDevice::LogicalDevice(const std::shared_ptr<VulkanContext> pVkContext, const DeviceInterfaceCreationInfo& deviceInterfaceCreationInfo)
    : pVkContext(pVkContext), physicalDevice(deviceInterfaceCreationInfo.physicalDevice), queueFamilyIndex(deviceInterfaceCreationInfo.queueFamilyIndex) {

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {
        deviceInterfaceCreationInfo.queueFamilyIndex.graphics.value(),
        deviceInterfaceCreationInfo.queueFamilyIndex.presentation.value()
    };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;

    if (deviceInterfaceCreationInfo.requestedDeviceExtensions.empty()) {
        createInfo.enabledExtensionCount = 0u;
        createInfo.ppEnabledExtensionNames = nullptr;
    }
    else {

        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceInterfaceCreationInfo.requestedDeviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceInterfaceCreationInfo.requestedDeviceExtensions.data();
    }

    const VkAllocationCallbacks* pAllocationCallbacks = pVkContext->getAllocationCallbacks();
    if (vkCreateDevice(this->physicalDevice, &createInfo, pAllocationCallbacks, &handle) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    debug_write("Logical Device successfully created");

    vkGetDeviceQueue(handle, deviceInterfaceCreationInfo.queueFamilyIndex.graphics.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(handle, deviceInterfaceCreationInfo.queueFamilyIndex.presentation.value(), 0, &presentationQueue);
}

LogicalDevice::~LogicalDevice() {
    if (handle == VK_NULL_HANDLE) return;

    const VkAllocationCallbacks* pAllocationCallbacks = pVkContext->getAllocationCallbacks();
    vkDestroyDevice(handle, pAllocationCallbacks);
    debug_write("Logical Device successfully destroyed");
}

VkPhysicalDevice LogicalDevice::getPhysicalDevice() const {
    return this->physicalDevice;
}

VkDevice LogicalDevice::getHandle() const {
    return this->handle;
}

QueueFamilyIndex LogicalDevice::getQueueFamilies() const {
    return this->queueFamilyIndex;
}

std::shared_ptr<LogicalDevice> DeviceFactory::Create(const std::shared_ptr<VulkanContext> pVkContext, const PhysicalDeviceSelectionInfo& deviceSelectionInfo) {
    DeviceInterfaceCreationInfo deviceInterfaceCreationInfo{};
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    uint32_t deviceCount = 0;

    VkInstance instance = pVkContext->getInstance();

    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        return VK_NULL_HANDLE;
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);

    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());


    for (const auto& device : devices) {
        if (!hasGeometryshader(device)) continue;

        if (!isRequestedDeviceType(device, deviceSelectionInfo.deviceType)) continue;

        if (deviceSelectionInfo.requestedDeviceExtensions.size() > 0
            && !hasRequestedExtensions(device, deviceSelectionInfo.requestedDeviceExtensions)) continue;

        if (!hasSwapChainSupport(device, deviceSelectionInfo.surface)) continue;

        QueueFamilyIndex queueFamilyIndices = findQueueFamilies(device, deviceSelectionInfo.surface);
        if (!queueFamilyIndices.isComplete()) continue;

        deviceInterfaceCreationInfo.physicalDevice = device;
        deviceInterfaceCreationInfo.queueFamilyIndex = queueFamilyIndices;
        break;
    }

    if (deviceInterfaceCreationInfo.physicalDevice == VK_NULL_HANDLE) {
        return VK_NULL_HANDLE;
    }

    deviceInterfaceCreationInfo.requestedDeviceExtensions = deviceSelectionInfo.requestedDeviceExtensions;

    std::shared_ptr<LogicalDevice> logicalDevice = std::make_shared<LogicalDevice>(pVkContext, deviceInterfaceCreationInfo);
    return logicalDevice;
}

bool DeviceFactory::isRequestedDeviceType(VkPhysicalDevice device, VkPhysicalDeviceType requestedDeviceType) {
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    return deviceProperties.deviceType == requestedDeviceType;
}


bool DeviceFactory::hasGeometryshader(VkPhysicalDevice device) {
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    return deviceFeatures.geometryShader;
}

bool DeviceFactory::hasRequestedExtensions(VkPhysicalDevice device, const std::vector<const char*>& requestedExtensions) {
    if (requestedExtensions.size() == 0) return true;

    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> extensionsToCheck(requestedExtensions.begin(), requestedExtensions.end());


    for (const auto& extension : availableExtensions) {
        extensionsToCheck.erase(extension.extensionName);
        if (extensionsToCheck.empty()) break;
    }

    return extensionsToCheck.empty();
}

bool DeviceFactory::hasSwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
    uint32_t formatCount;
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
    return (formatCount > 0 && presentModeCount > 0);
}

QueueFamilyIndex DeviceFactory::findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
    QueueFamilyIndex indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphics = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

        if (presentSupport) {
            indices.presentation = i;
        }

        if (indices.isComplete()) {
            break;
        }

        i++;
    }

    return indices;
}
