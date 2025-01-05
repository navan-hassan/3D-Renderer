#ifndef DEVICE_ADAPTER_HPP
#define DEVICE_ADAPTER_HPP

#include <GLFW/glfw3.h>
#include <vector>
#include <optional>
#include <device.hpp>

class DeviceSelector {

public:
    std::vector<PhysicalDevice> availablePhysicalDevices;
    uint32_t deviceCount = 0;
    void getPhysicalDevices(VkInstance* instance);
    void getPhysicalDevices(VkInstance* instance, VkSurfaceKHR* surface);
    void printDeviceInfoToScreen();

};

#endif