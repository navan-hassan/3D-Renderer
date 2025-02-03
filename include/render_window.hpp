#ifndef RENDER_WINDOW_HPP
#define RENDER_WINDOW_HPP

#include <memory>
#include <stdexcept>
#include <GLFW/glfw3.h>

#include <constants.hpp>
#include <vulkan_context.hpp>
#include <debug_utilities.hpp>

constexpr size_t DEFAULT_WIDTH = 800;
constexpr size_t DEFAULT_HEIGHT = 600;

struct WindowCreationInfo {
	size_t width = DEFAULT_WIDTH;
	size_t height = DEFAULT_HEIGHT;
	const char* title = WINDOW_TITLE;
};

// Wrapper class for interfacing with a GLFW window
class Window {
public:
	Window(const std::shared_ptr<VulkanContext> pVkContext)
		: pVkContext(pVkContext) {
		WindowCreationInfo windowCreationInfo{};

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		handle = glfwCreateWindow(
			windowCreationInfo.width,
			windowCreationInfo.height,
			windowCreationInfo.title,
			nullptr,
			nullptr
		);

		VkInstance instance = pVkContext->getInstance();
		const VkAllocationCallbacks* pAllocationCallbacks = pVkContext->getAllocationCallbacks();
		VkSurfaceKHR vkSurface;
		if (glfwCreateWindowSurface(instance, handle, nullptr, &vkSurface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}
		surface = vkSurface;
		debug_write("VK Surface successfully created");
	}

	Window(const std::shared_ptr<VulkanContext> pVkContext, const WindowCreationInfo& windowCreationInfo)
		: pVkContext(pVkContext) {

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		handle = glfwCreateWindow(
			windowCreationInfo.width,
			windowCreationInfo.height,
			windowCreationInfo.title,
			nullptr,
			nullptr
		);

		VkInstance instance = pVkContext->getInstance();
		const VkAllocationCallbacks* pAllocationCallbacks = pVkContext->getAllocationCallbacks();
		VkSurfaceKHR vkSurface;
		if (glfwCreateWindowSurface(instance, handle, nullptr, &vkSurface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}
		surface = vkSurface;
		debug_write("VK Surface successfully created");
	}

	~Window() {
		if (surface != VK_NULL_HANDLE) {
			VkInstance instance = pVkContext->getInstance();
			const VkAllocationCallbacks* pAllocationCallbacks = pVkContext->getAllocationCallbacks();
			vkDestroySurfaceKHR(instance, surface, pAllocationCallbacks);
			debug_write("VK Surface successfully destroyed");
		}

		if (handle != VK_NULL_HANDLE) {
			glfwDestroyWindow(handle);
		}

		debug_write("GLFW window successfully destroyed");
	}

	void mainLoop() {
		while (!glfwWindowShouldClose(handle)) {
			glfwPollEvents();
		}
	}

	VkSurfaceKHR getSurface() const {
		return this->surface;
	}

private:
	std::shared_ptr<VulkanContext> pVkContext;
	VkSurfaceKHR surface;
	GLFWwindow* handle;
};

#endif