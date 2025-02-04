#include <renderer.hpp>

Renderer::Renderer(const std::shared_ptr<VulkanContext> pVkContext) : 
	pVkContext(pVkContext), pWindow(std::shared_ptr<Window>(new Window(pVkContext))) {
	PhysicalDeviceSelectionInfo deviceSelectionInfo{};
	deviceSelectionInfo.surface = pWindow->getSurface();
	deviceSelectionInfo.deviceType = VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
	deviceSelectionInfo.requestedDeviceExtensions.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

	this->pLogicalDevice = DeviceFactory::Create(pVkContext, deviceSelectionInfo);
	if (this->pLogicalDevice == VK_NULL_HANDLE) {
		throw std::runtime_error("ERROR: Failed to create logical device!");
	}

	FramebufferDimensions framebufferDimensions = pWindow->getFramebufferSize();

	SwapChainCreationInfo swapChainCreationInfo{};
	swapChainCreationInfo.vkPresentModeKHR = VK_PRESENT_MODE_MAILBOX_KHR;
	swapChainCreationInfo.surfaceFormat.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	swapChainCreationInfo.surfaceFormat.format = VK_FORMAT_B8G8R8A8_SRGB;

	pSwapChain = std::shared_ptr<SwapChain>(new SwapChain(pVkContext, pWindow, pLogicalDevice, swapChainCreationInfo));
}

void Renderer::run() {
	pWindow->mainLoop();
}