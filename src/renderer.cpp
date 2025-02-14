#include <renderer.hpp>
#include <shader.hpp>

Renderer::Renderer(const std::shared_ptr<VulkanContext> pVkContext) 
	: pVkContext(pVkContext), pWindow(std::make_unique<Window>(pVkContext)) {}

VkResult Renderer::init() {

	if (createDevice() != VK_SUCCESS) {
		debug_write("COULD NOT CREATE LOGICAL DEVICE");
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	if (createSwapChain() != VK_SUCCESS) {
		debug_write("COULD NOT CREATE SWAP CHAIN");
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	return VK_SUCCESS;
}

VkResult Renderer::createDevice() {
	PhysicalDeviceSelectionInfo deviceSelectionInfo{};
	deviceSelectionInfo.surface = pWindow->getSurface();
	deviceSelectionInfo.deviceType = VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
	deviceSelectionInfo.requestedDeviceExtensions.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

	this->pLogicalDevice = DeviceFactory::Create(pVkContext, deviceSelectionInfo);
	if (this->pLogicalDevice == VK_NULL_HANDLE) {
		debug_write("COULD NOT CREATE LOGICAL DEVICE");
		return VK_ERROR_INITIALIZATION_FAILED;
		//throw std::runtime_error("ERROR: Failed to create logical device!");
	}
	return VK_SUCCESS;
}

VkResult Renderer::createSwapChain() {
	if (!pLogicalDevice) {
		debug_write("LOGICAL DEVICE NOT INITIALIZED YET!");
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	SwapChainCreationInfo swapChainCreationInfo{};
	swapChainCreationInfo.presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
	swapChainCreationInfo.surfaceFormat.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	swapChainCreationInfo.surfaceFormat.format = VK_FORMAT_B8G8R8A8_SRGB;
	swapChainCreationInfo.surface = pWindow->getSurface();
	swapChainCreationInfo.framebufferDimensions = pWindow->getFramebufferSize();

	pSwapChain = std::make_unique<SwapChain>(pVkContext, pLogicalDevice);
	if (pSwapChain->init(swapChainCreationInfo) != VK_SUCCESS) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}
	//pSwapChain->createImageViews();

	return VK_SUCCESS;
}

VkResult Renderer::buildPipeline() {
	auto vertShaderCode = readFile(VERTEX_SHADER_PATH);
	auto fragShaderCode = readFile(FRAGMENT_SHADER_PATH);

	std::shared_ptr<ShaderModule> vertexModule = std::shared_ptr<ShaderModule>(new ShaderModule(VK_SHADER_STAGE_VERTEX_BIT, vertShaderCode, pVkContext, pLogicalDevice));
	std::shared_ptr<ShaderModule> fragmentModule = std::shared_ptr<ShaderModule>(new ShaderModule(VK_SHADER_STAGE_VERTEX_BIT, fragShaderCode, pVkContext, pLogicalDevice));

	return VK_SUCCESS;
}

void Renderer::run() {
	pWindow->mainLoop();
}