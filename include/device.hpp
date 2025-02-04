#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <GLFW/glfw3.h>
#include <vector>
#include <optional>
#include <string>

#include <vulkan_context.hpp>

struct QueueFamilyIndex {
    std::optional<uint32_t> graphics;
    std::optional<uint32_t> presentation;
    bool isComplete() const;
};

struct PhysicalDeviceSelectionInfo {
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    VkPhysicalDeviceType deviceType = VK_PHYSICAL_DEVICE_TYPE_OTHER;
    std::vector<const char*> requestedDeviceExtensions;
    ~PhysicalDeviceSelectionInfo();
};

struct DeviceInterfaceCreationInfo {
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    QueueFamilyIndex queueFamilyIndex;
    std::vector<const char*> requestedDeviceExtensions;
    ~DeviceInterfaceCreationInfo();
};

class LogicalDevice {
public:
    LogicalDevice(const std::shared_ptr<VulkanContext> pVkContext, const DeviceInterfaceCreationInfo& deviceInterfaceCreationInfo);
    ~LogicalDevice();
    VkPhysicalDevice getPhysicalDevice() const;
    VkDevice getHandle() const;
    QueueFamilyIndex getQueueFamilies() const;
private:
    std::shared_ptr<VulkanContext> pVkContext;
    VkDevice handle = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkQueue graphicsQueue = VK_NULL_HANDLE;
    VkQueue presentationQueue = VK_NULL_HANDLE;
    QueueFamilyIndex queueFamilyIndex;
};

class DeviceFactory {
public:
    static std::shared_ptr<LogicalDevice> Create(const std::shared_ptr<VulkanContext> pVkContext, const PhysicalDeviceSelectionInfo& deviceSelectionInfo);
private:
    static bool isRequestedDeviceType(VkPhysicalDevice device, VkPhysicalDeviceType requestedDeviceType);
    static bool hasGeometryshader(VkPhysicalDevice device);
    static bool hasRequestedExtensions(VkPhysicalDevice device, const std::vector<const char*>& requestedExtensions);
    static bool hasSwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
    static QueueFamilyIndex findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
};

#endif
