#ifndef RENDER_WINDOW_HPP
#define RENDER_WINDOW_HPP

#include <memory>

#include <vulkan_context.hpp>

struct WindowCreationInfo {
	WindowCreationInfo();
	size_t width;
	size_t height;
	const char* title;
};

struct FramebufferDimensions {
	int height;
	int width;
};

class Window {
public:
	Window(const std::shared_ptr<VulkanContext> pVkContext, std::shared_ptr<GLFWContext> pGLFWContext);
	Window(const std::shared_ptr<VulkanContext> pVkContext, std::shared_ptr<GLFWContext> pGLFWContext, const WindowCreationInfo& windowCreationInfo);
	~Window();
	void mainLoop();
	void close() const;
	bool isOpen() const;
	VkSurfaceKHR getSurface() const;
	FramebufferDimensions getFramebufferSize() const;
private:
	std::shared_ptr<VulkanContext> pVkContext;
	std::shared_ptr<GLFWContext> pGLFWContext;
	VkSurfaceKHR surface;
	GLFWwindow* handle;
};

#endif