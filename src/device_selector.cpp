#include <device_selector.hpp>

#include <stdexcept>
#include <optional>
#include <iostream>


void DeviceSelector::getPhysicalDevices(VkInstance* instance)
{
    vkEnumeratePhysicalDevices(*instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(*instance, &deviceCount, devices.data());
    availablePhysicalDevices = std::vector<PhysicalDevice>(deviceCount);
    size_t i = 0;
    for (const auto& device : devices) {
        availablePhysicalDevices.at(i) = PhysicalDevice::PhysicalDevice(device);
        i++;
    }
}

void DeviceSelector::getPhysicalDevices(VkInstance* instance, VkSurfaceKHR* surface)
{
    vkEnumeratePhysicalDevices(*instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(*instance, &deviceCount, devices.data());
    availablePhysicalDevices = std::vector<PhysicalDevice>(deviceCount);
    size_t i = 0;
    for (const auto& device : devices) {
        availablePhysicalDevices.at(i) = PhysicalDevice::PhysicalDevice(device, surface);
        i++;
    }
}

void DeviceSelector::printDeviceInfoToScreen() {

    if (deviceCount == 0) return;

    std::cout << "AVAILABLE PHYSICAL DEVICES" << std::endl;
    std::cout << "--------------------" << std::endl;

    for (auto& physicalDevice : availablePhysicalDevices) {
        physicalDevice.printDeviceInfo();
    }
}
