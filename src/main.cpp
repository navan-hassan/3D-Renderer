#include <renderer.hpp>
#include <cstdlib>

int main() {
	VulkanContextInitOptions vkContextInitOptions{};
	vkContextInitOptions.enableValidationLayers = true;
	vkContextInitOptions.validationLayers.push_back("VK_LAYER_KHRONOS_validation");
	
	std::shared_ptr<VulkanContext> vkContext = std::make_shared<VulkanContext>(vkContextInitOptions);
	Renderer renderer = Renderer::Renderer(vkContext);

	if (renderer.init() != VK_SUCCESS) {
		debug_write("FAILED TO INITIALIZE RENDERER!");
		return EXIT_FAILURE;
	}
	//renderer.init();
	renderer.run();

	return EXIT_SUCCESS;
}