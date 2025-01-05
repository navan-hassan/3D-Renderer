#include <device.hpp>
#include <iostream>
#include <debug_utilities.hpp>

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

PhysicalDevice::PhysicalDevice() {}

PhysicalDevice::PhysicalDevice(VkPhysicalDevice device) {

    this->device = device;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    findQueueFamilies();
}

PhysicalDevice::PhysicalDevice(VkPhysicalDevice device, VkSurfaceKHR* surface) {

    this->device = device;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    findQueueFamilies(surface);
}

void PhysicalDevice::findQueueFamilies() {

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    VkQueueFlagBits graphicsQueueBit = VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT;

    auto selectedItem = std::find_if(
        queueFamilies.begin(), 
        queueFamilies.end(),
        [graphicsQueueBit](const VkQueueFamilyProperties& queueFamilyProperties) {
            return queueFamilyProperties.queueFlags & graphicsQueueBit;
        }
    );

    if (selectedItem != queueFamilies.end()) {
        size_t index = std::distance(queueFamilies.begin(), selectedItem);
        queueFamilyIndex.graphics = index;
    }
}

void PhysicalDevice::findQueueFamilies(VkSurfaceKHR* surface) {

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    size_t index = 0;
    VkBool32 hasPresentationSupport = false;
    for (const auto& queueFamily : queueFamilies) {

        if (hasGraphicsQueueFamily() && hasPresentationQueueFamily()) {
            break;
        }

        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            queueFamilyIndex.graphics = index;
            index++;
            continue;
        }

        vkGetPhysicalDeviceSurfaceSupportKHR(device, index, *surface, &hasPresentationSupport);
        if (hasPresentationSupport) {
            queueFamilyIndex.presentation = index;
            index++;
            continue;
        }

        index++;
    }
}

void PhysicalDevice::printDeviceInfo() {

    std::cout << "DEVICE INFO: "  << std::endl;
    std::cout << "\t" << "DEVICE NAME: " << getDeviceName() << std::endl;
    std::cout << "\t" << "DEVICE TYPE: " << getDeviceType() << std::endl;
    std::cout << "\t" << "DRIVER VERSION: " << deviceProperties.driverVersion << std::endl;
    std::cout << "--------------------" << std::endl;
}


std::string PhysicalDevice::getDeviceType() {

    switch (deviceProperties.deviceType) {
    case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
        return "DISCRETE GPU";
    case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
        return "INTEGRATED GPU";
    case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
        return "VIRTUAL GPU";
    case VK_PHYSICAL_DEVICE_TYPE_CPU:
        return "CPU";
    case VK_PHYSICAL_DEVICE_TYPE_OTHER:
        return "OTHER";
    default:
        return "UNKNOWN";
    }
}

std::string PhysicalDevice::getDeviceName() {

    if (device == VK_NULL_HANDLE) {
        return "UNINITIALIZED";
    }

    return deviceProperties.deviceName;
}

bool PhysicalDevice::hasGraphicsQueueFamily() {
    return queueFamilyIndex.graphics.has_value();
}
bool PhysicalDevice::hasPresentationQueueFamily() {
    return PhysicalDevice::queueFamilyIndex.presentation.has_value();
}

uint32_t PhysicalDevice::getGraphicsQueueFamily() {
    return queueFamilyIndex.graphics.value();
}

uint32_t PhysicalDevice::getPresentationQueueFamily() {
    return queueFamilyIndex.presentation.value();
}

void LogicalDevice::initialize(PhysicalDevice physicalDevice) {

    VkDeviceCreateInfo deviceCreateInfo{};

    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();

    VkPhysicalDeviceFeatures physicalDeviceFeatures{};
    deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;

    if (ENABLE_VALIDATION_LAYERS) {
        deviceCreateInfo.enabledLayerCount = NUM_VALIDATION_LAYERS;
        deviceCreateInfo.ppEnabledLayerNames = VALIDATION_LAYERS;
    }
    else {
        deviceCreateInfo.enabledLayerCount = 0;
    }

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    float queuePriority = 1.0f;
    if (physicalDevice.hasGraphicsQueueFamily()) {
        VkDeviceQueueCreateInfo graphicsQueueCreateInfo{};
        graphicsQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        graphicsQueueCreateInfo.queueFamilyIndex = physicalDevice.getGraphicsQueueFamily();
        graphicsQueueCreateInfo.queueCount = 1;
        graphicsQueueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(graphicsQueueCreateInfo);
    }

    if (physicalDevice.hasPresentationQueueFamily()) {
        VkDeviceQueueCreateInfo presentationQueueCreateInfo{};
        presentationQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        presentationQueueCreateInfo.queueFamilyIndex = physicalDevice.getPresentationQueueFamily();
        presentationQueueCreateInfo.queueCount = 1;
        presentationQueueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(presentationQueueCreateInfo);
    }

    if (!queueCreateInfos.empty()) {
        deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
        deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    }
    else {
        deviceCreateInfo.pQueueCreateInfos = nullptr;
        deviceCreateInfo.queueCreateInfoCount = 0;
    }

    if (vkCreateDevice(physicalDevice.device, &deviceCreateInfo, nullptr, &this->device) != VK_SUCCESS) {
        throw std::runtime_error("FAILED TO CREATE LOGICAL DEVICE!");
    }
    debug_write("SUCCESSFULLY CREATED LOGICAL DEVICE");

    vkGetDeviceQueue(this->device, physicalDevice.getGraphicsQueueFamily(), 0, &this->graphicsQueue);
    debug_write("SUCCESSFULLY CREATED GRAPHICS QUEUE");

    vkGetDeviceQueue(this->device, physicalDevice.getPresentationQueueFamily(), 0, &this->presentationQueue);
    debug_write("SUCCESSFULLY CREATED PRESENTATION QUEUE");
}

void LogicalDevice::destroy() const {

    if (this->device == VK_NULL_HANDLE) return;

    vkDestroyDevice(this->device, nullptr);
    debug_write("SUCCESSFULLY DESTROYED DEVICE");
}