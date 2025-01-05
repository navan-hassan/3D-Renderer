#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <GLFW/glfw3.h>
#include <vector>
#include <optional>
#include <string>

struct QueueFamilyIndex {
    std::optional<uint32_t> graphics;
    std::optional<uint32_t> presentation;
};

class PhysicalDevice {
public:

    VkPhysicalDevice device = VK_NULL_HANDLE;

    PhysicalDevice();
    PhysicalDevice(VkPhysicalDevice device);
    PhysicalDevice(VkPhysicalDevice device, VkSurfaceKHR* surface);
    bool hasGraphicsQueueFamily();
    bool hasPresentationQueueFamily();
    std::string getDeviceType();
    std::string getDeviceName();
    void printDeviceInfo();
    uint32_t getGraphicsQueueFamily();
    uint32_t getPresentationQueueFamily();
private:

    VkPhysicalDeviceProperties deviceProperties{};
    VkPhysicalDeviceFeatures deviceFeatures{};
    uint32_t score = 0;
    QueueFamilyIndex queueFamilyIndex;

    void findQueueFamilies();
    void findQueueFamilies(VkSurfaceKHR* surface);
};

class LogicalDevice {
public:
    void initialize(PhysicalDevice physicalDevice);
    void destroy() const;

private:
    VkDevice device = VK_NULL_HANDLE;
    VkQueue graphicsQueue = VK_NULL_HANDLE;
    VkQueue presentationQueue = VK_NULL_HANDLE;
};

#endif
