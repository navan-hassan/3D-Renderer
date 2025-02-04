#ifndef SWAP_CHAIN_HPP
#define SWAP_CHAIN_HPP
#include <GLFW/glfw3.h>
#include <vector>
#include <algorithm>

#include <device.hpp>
#include <window.hpp>

struct SwapChainCreationInfo {
	VkPresentModeKHR vkPresentModeKHR;
	VkSurfaceFormatKHR surfaceFormat;
};

class SwapChain {
public:
	SwapChain(std::shared_ptr<VulkanContext> pVkContext, std::shared_ptr<Window> pWindow, std::shared_ptr<LogicalDevice> pLogicalDevice, const SwapChainCreationInfo& creationInfo);
	~SwapChain();
private:
	std::shared_ptr<VulkanContext> pVkContext;
	std::shared_ptr<LogicalDevice> pLogicalDevice;
	std::shared_ptr<Window> pWindow;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	VkSwapchainKHR swapChain = VK_NULL_HANDLE;
	
	static bool hasRequestedPresentMode(const VkPhysicalDevice& physicalDevice, const VkPresentModeKHR& requestedPresentMode, const VkSurfaceKHR& surface);
	static VkSurfaceFormatKHR selectSurfaceFormat(const VkPhysicalDevice& physicalDevice, const VkSurfaceFormatKHR& requestedSurfaceFormat, const VkSurfaceKHR& surface);
	static VkExtent2D setSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, size_t framebufferWidth, size_t framebufferHeight, const VkSurfaceKHR& surface, const VkPhysicalDevice& physicalDevice);
};
#endif