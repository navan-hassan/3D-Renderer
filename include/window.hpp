#ifndef RENDER_WINDOW_HPP
#define RENDER_WINDOW_HPP

#include <GLFW/glfw3.h>

#include <memory>

#include <vulkan_context.hpp>


constexpr size_t DEFAULT_WIDTH = 800;
constexpr size_t DEFAULT_HEIGHT = 600;

struct WindowCreationInfo {
	size_t width = DEFAULT_WIDTH;
	size_t height = DEFAULT_HEIGHT;
	const char* title = WINDOW_TITLE;
};

struct FramebufferDimensions {
	int height;
	int width;
};

// Wrapper class for interfacing with a GLFW window
class Window {
public:
	Window(const std::shared_ptr<VulkanContext> pVkContext);
	Window(const std::shared_ptr<VulkanContext> pVkContext, const WindowCreationInfo& windowCreationInfo);
	~Window();
	void mainLoop();
	VkSurfaceKHR getSurface() const;
	FramebufferDimensions getFramebufferSize() const;
private:
	std::shared_ptr<VulkanContext> pVkContext;
	VkSurfaceKHR surface;
	GLFWwindow* handle;
};

#endif