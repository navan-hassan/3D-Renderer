#ifndef RENDER_HPP
#define RENDER_HPP

#include <GLFW/glfw3.h>
#include <render_window.hpp>
#include <vulkan_context.hpp>
#include <debug_utilities.hpp>
#include <device.hpp>

// Class that handles logic for rendering on a GLFW window 
class Renderer {
public:
	Renderer(const std::shared_ptr<VulkanContext> pVkContext) : pVkContext(pVkContext), window(pVkContext) {
		PhysicalDeviceSelectionInfo deviceSelectionInfo{};
		deviceSelectionInfo.surface = this->window.getSurface();
		deviceSelectionInfo.deviceType = VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
		deviceSelectionInfo.requestedDeviceExtensions.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		this->logicalDevice = DeviceFactory::Create(pVkContext, deviceSelectionInfo);
		if (this->logicalDevice == VK_NULL_HANDLE) {
			throw std::runtime_error("ERROR: Failed to create logical device!");
		}
	}

	void run() {
		window.mainLoop();
	}

private:
	std::shared_ptr<VulkanContext> pVkContext;
	std::shared_ptr<LogicalDevice> logicalDevice;
	Window window;
};

#endif