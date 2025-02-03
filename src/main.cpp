#include <renderer.hpp>
#include <cstdlib>

int main() {

	VulkanContextInitOptions vkContextInitOptions{};
	vkContextInitOptions.validationLayers.push_back("VK_LAYER_KHRONOS_validation");
	std::shared_ptr<VulkanContext> vkContext = std::shared_ptr<VulkanContext>(new VulkanContext(vkContextInitOptions));
	Renderer renderer = Renderer::Renderer(vkContext);
	renderer.run();
	return EXIT_SUCCESS;
}