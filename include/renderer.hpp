#ifndef RENDER_HPP
#define RENDER_HPP

#include <memory>

#include <window.hpp>
#include <device.hpp>
#include <swap_chain.hpp>

class Renderer {
public:
    Renderer(const std::shared_ptr<VulkanContext> pVkContext, std::shared_ptr<Window> pWindow);
    VkResult init();
    void run();
private:
    std::shared_ptr<VulkanContext> pVkContext;
    std::shared_ptr<LogicalDevice> pLogicalDevice;
    std::shared_ptr<Window> pWindow;
    std::unique_ptr<SwapChain> pSwapChain;
    VkResult createDevice();
    VkResult createSwapChain();
    VkResult buildPipeline();
};

#endif