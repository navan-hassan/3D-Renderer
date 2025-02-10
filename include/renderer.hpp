#ifndef RENDER_HPP
#define RENDER_HPP

#include <memory>

#include <window.hpp>
#include <device.hpp>
#include <swap_chain.hpp>

// Class that handles logic for rendering on a GLFW window 
class Renderer {
public:
	Renderer(const std::shared_ptr<VulkanContext> pVkContext);
	VkResult init();
	void run();

private:
	std::shared_ptr<VulkanContext> pVkContext = nullptr;
	std::shared_ptr<LogicalDevice> pLogicalDevice;
	std::unique_ptr<Window> pWindow;
	std::unique_ptr<SwapChain> pSwapChain;

	VkResult createDevice();
	VkResult createSwapChain();
	VkResult buildPipeline();
};

#endif