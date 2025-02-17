#ifndef SWAP_CHAIN_HPP
#define SWAP_CHAIN_HPP

#include <vector>

#include <vulkan_context.hpp>
#include <device.hpp>
#include <window.hpp>

struct SwapChainCreationInfo {
	VkPresentModeKHR presentMode;
	VkSurfaceFormatKHR surfaceFormat;
	VkSurfaceKHR surface;
	FramebufferDimensions framebufferDimensions;
};

class SwapChain {
public:
	SwapChain(std::shared_ptr<VulkanContext> pVkContext, std::shared_ptr<LogicalDevice> pLogicalDevice);
	~SwapChain();
	void createImageViews();
	VkResult init(const SwapChainCreationInfo& creationInfo);
private:
	std::shared_ptr<VulkanContext> pVkContext;
	std::shared_ptr<LogicalDevice> pLogicalDevice;
	//std::shared_ptr<Window> pWindow;
	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;
	VkFormat swapChainImageFormat = VK_FORMAT_UNDEFINED;
	VkExtent2D swapChainExtent{};
	VkSwapchainKHR swapChain = VK_NULL_HANDLE;
	
	static bool hasRequestedPresentMode(const VkPhysicalDevice& physicalDevice, const VkPresentModeKHR& requestedPresentMode, const VkSurfaceKHR& surface);
	static VkSurfaceFormatKHR selectSurfaceFormat(const VkPhysicalDevice& physicalDevice, const VkSurfaceFormatKHR& requestedSurfaceFormat, const VkSurfaceKHR& surface);
	static VkExtent2D setSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, size_t framebufferWidth, size_t framebufferHeight, const VkSurfaceKHR& surface, const VkPhysicalDevice& physicalDevice);
};
#endif