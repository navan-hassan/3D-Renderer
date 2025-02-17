#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <vulkan_context.hpp>
#include <window.hpp>
#include <renderer.hpp>

class Engine {
public:
    Engine();
    bool init(bool debugMode = false);
    void eventLoop();
private:
    std::shared_ptr<GLFWContext> pGLFWContext;
    std::shared_ptr<VulkanContext> pVkContext;
    std::shared_ptr<Window> pWindow;
    std::unique_ptr<Renderer> pRenderingContext;
};

#endif