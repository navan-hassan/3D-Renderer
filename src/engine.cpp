#include <engine.hpp>
#include <constants.hpp>
#include <logging_utilities.hpp>

Engine::Engine() {}

bool Engine::init(bool debugMode) {
	pGLFWContext = GLFWContext::createSharedInstance();
	VulkanContextInitOptions vkContextInitOptions{};
	if (debugMode) {
		vkContextInitOptions.enableValidationLayers = true;
		vkContextInitOptions.validationLayers.push_back(constants::VK_LAYER_KHRONOS_VALIDATION);
	}
	pVkContext = std::make_shared<VulkanContext>(vkContextInitOptions, pGLFWContext);
	pWindow = std::make_shared<Window>(pVkContext, pGLFWContext);
	pRenderingContext = std::make_unique<Renderer>(pVkContext, pWindow);

	if (pRenderingContext->init() != VK_SUCCESS) {
		logging::ERROR("COULD NOT INITIALIZE RENDERER!");
		return false;
	}

	logging::DEBUG("SUCCESSFULLY INITIALIZED RENDERER");
	return true;
}

void Engine::eventLoop() {
	while (pWindow->isOpen()) {
		glfwPollEvents();
	}
}