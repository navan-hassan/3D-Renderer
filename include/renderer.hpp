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
	void run();
private:
	std::shared_ptr<VulkanContext> pVkContext;
	std::shared_ptr<Window> pWindow;
	std::shared_ptr<LogicalDevice> pLogicalDevice;
	std::shared_ptr<SwapChain> pSwapChain;
};

#endif