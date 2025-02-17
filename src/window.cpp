#include <window.hpp>
#include <constants.hpp>
#include <logging_utilities.hpp>

#include <stdexcept>

static constexpr size_t DEFAULT_WIDTH = 800;
static constexpr size_t DEFAULT_HEIGHT = 600;

WindowCreationInfo::WindowCreationInfo()
    : title(constants::WINDOW_TITLE), width(DEFAULT_WIDTH), height(DEFAULT_HEIGHT) {}

Window::Window(const std::shared_ptr<VulkanContext> pVkContext, std::shared_ptr<GLFWContext> pGLFWContext)
    : pVkContext(pVkContext), pGLFWContext(pGLFWContext) {
    WindowCreationInfo windowCreationInfo{};

    handle = glfwCreateWindow(
        windowCreationInfo.width,
        windowCreationInfo.height,
        windowCreationInfo.title,
        nullptr,
        nullptr
    );

    if (handle == NULL) {
        throw std::runtime_error("failed to create GLFW window!");
    }
    logging::DEBUG("SUCCESSFULLY CREATED GLFW WINDOW");


    VkInstance instance = pVkContext->getInstance();
    const VkAllocationCallbacks* pAllocationCallbacks = pVkContext->getAllocationCallbacks();
    VkSurfaceKHR vkSurface;
    if (glfwCreateWindowSurface(instance, handle, nullptr, &vkSurface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
    surface = vkSurface;
    logging::DEBUG("SUCCESSFULLY CREATED VK SURFACE");
}

Window::Window(const std::shared_ptr<VulkanContext> pVkContext, std::shared_ptr<GLFWContext> pGLFWContext, const WindowCreationInfo& windowCreationInfo)
    : pVkContext(pVkContext), pGLFWContext(pGLFWContext) {

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
    logging::DEBUG("SUCCESSFULLY CREATED VK SURFACE");
}

Window::~Window() {
    if (surface) {
        VkInstance instance = pVkContext->getInstance();
        const VkAllocationCallbacks* pAllocationCallbacks = pVkContext->getAllocationCallbacks();
        vkDestroySurfaceKHR(instance, surface, pAllocationCallbacks);
        logging::DEBUG("SUCCESSFULLY DESTROYED VK SURFACE");
    }

    if (handle != VK_NULL_HANDLE) {
        glfwDestroyWindow(handle);
        logging::DEBUG("SUCCESSFULLY DESTROYED GLFW WINDOW");
    }
}

bool Window::isOpen() const {
    if (handle == nullptr) {
        return false;
    }

    return !(glfwWindowShouldClose(handle));
}

void Window::close() const {
    if (handle == nullptr) return;

    glfwSetWindowShouldClose(handle, true);
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