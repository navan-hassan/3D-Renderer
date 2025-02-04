#include <window.hpp>

#include <stdexcept>

#include <constants.hpp>
#include <debug_utilities.hpp>

Window::Window(const std::shared_ptr<VulkanContext> pVkContext)
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

Window::Window(const std::shared_ptr<VulkanContext> pVkContext, const WindowCreationInfo& windowCreationInfo)
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

Window::~Window() {
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

void Window::mainLoop() {
	while (!glfwWindowShouldClose(handle)) {
		glfwPollEvents();
	}
}

VkSurfaceKHR Window::getSurface() const {
	return this->surface;
}

FramebufferDimensions Window::getFramebufferSize() const {
	FramebufferDimensions framebufferDimensions{};
	glfwGetFramebufferSize(handle, &framebufferDimensions.width, &framebufferDimensions.height);
	return framebufferDimensions;
}