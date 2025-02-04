#include <swap_chain.hpp>

SwapChain::SwapChain(std::shared_ptr<VulkanContext> pVkContext, std::shared_ptr<Window> pWindow, std::shared_ptr<LogicalDevice> pLogicalDevice, const SwapChainCreationInfo& creationInfo)
	: pVkContext(pVkContext), pWindow(pWindow), pLogicalDevice(pLogicalDevice) {
	VkPhysicalDevice physicalDevice = pLogicalDevice->getPhysicalDevice();

	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	std::vector<VkSurfaceFormatKHR> surfaceFormats;
	std::vector<VkPresentModeKHR> presentModes;
	VkSurfaceKHR surface = pWindow->getSurface();
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);

	uint32_t imageCount = surfaceCapabilities.minImageCount + 1;
	if (surfaceCapabilities.maxImageCount > 0 && imageCount > surfaceCapabilities.maxImageCount) {
		imageCount = surfaceCapabilities.maxImageCount;
	}
	
	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;

	createInfo.minImageCount = imageCount;

	VkSurfaceFormatKHR surfaceFormat = selectSurfaceFormat(physicalDevice, creationInfo.surfaceFormat, surface);
	FramebufferDimensions framebufferDimensions = pWindow->getFramebufferSize();
	swapChainExtent = setSwapExtent(surfaceCapabilities, static_cast<uint32_t>(framebufferDimensions.width), static_cast<uint32_t>(framebufferDimensions.height), surface, physicalDevice);

	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = swapChainExtent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndex queueFamilies = pLogicalDevice->getQueueFamilies();

	uint32_t queueFamilyIndices[] = {
		queueFamilies.graphics.value(),
		queueFamilies.presentation.value()
	};

	if (queueFamilies.graphics != queueFamilies.presentation) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	createInfo.preTransform = surfaceCapabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

	createInfo.presentMode = (hasRequestedPresentMode(physicalDevice, creationInfo.vkPresentModeKHR, surface))
		? creationInfo.vkPresentModeKHR : VK_PRESENT_MODE_FIFO_KHR;

	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	const VkAllocationCallbacks* pAllocationCallbacks = pVkContext->getAllocationCallbacks();
	VkDevice logicaldevice = pLogicalDevice->getHandle();

	if (vkCreateSwapchainKHR(logicaldevice, &createInfo, pAllocationCallbacks, &swapChain) != VK_SUCCESS) {
		throw std::runtime_error("failed to create swap chain!");
	}

	vkGetSwapchainImagesKHR(logicaldevice, swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(logicaldevice, swapChain, &imageCount, swapChainImages.data());

	swapChainImageFormat = surfaceFormat.format;

	debug_write("Successfully created swap chain");
}

SwapChain::~SwapChain() {
	if (swapChain == VK_NULL_HANDLE) return;

	VkDevice logicaldevice = pLogicalDevice->getHandle();
	const VkAllocationCallbacks* pAllocationCallbacks = pVkContext->getAllocationCallbacks();

	vkDestroySwapchainKHR(logicaldevice, swapChain, pAllocationCallbacks);
	debug_write("Successfully destroyed swap chain");
}

bool SwapChain::hasRequestedPresentMode(const VkPhysicalDevice& physicalDevice, const VkPresentModeKHR& requestedPresentMode, const VkSurfaceKHR& surface) {
	std::vector<VkPresentModeKHR> presentModes;

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data());
	}

	auto foundPresentMode = std::find_if(
		presentModes.begin(),
		presentModes.end(),
		[requestedPresentMode](const VkPresentModeKHR& vkPresentModeKHR) {
			return vkPresentModeKHR == requestedPresentMode;
		}
	) != presentModes.end();

	return foundPresentMode;
}

VkSurfaceFormatKHR SwapChain::selectSurfaceFormat(const VkPhysicalDevice& physicalDevice, const VkSurfaceFormatKHR& requestedSurfaceFormat, const VkSurfaceKHR& surface) {
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);

	std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, surfaceFormats.data());

	auto foundSurfaceFormat = std::find_if(
		surfaceFormats.begin(),
		surfaceFormats.end(),
		[requestedSurfaceFormat](const VkSurfaceFormatKHR& vkSurfaceFormat) {
			return vkSurfaceFormat.colorSpace == requestedSurfaceFormat.colorSpace
				&& vkSurfaceFormat.format == requestedSurfaceFormat.format;
		}
	) != surfaceFormats.end();

	if (!foundSurfaceFormat) {
		return surfaceFormats.at(0);
	}

	return requestedSurfaceFormat;
}

VkExtent2D SwapChain::setSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, size_t framebufferWidth, size_t framebufferHeight, const VkSurfaceKHR& surface, const VkPhysicalDevice& physicalDevice) {

	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	}

	VkExtent2D actualExtent = { static_cast<uint32_t>(framebufferWidth), static_cast<uint32_t>(framebufferHeight) };

	actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
	actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

	return actualExtent;
}
