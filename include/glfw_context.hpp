#ifndef GLFW_CONTEXT_HPP
#define GLFW_CONTEXT_HPP

#include <GLFW/glfw3.h>

#include <vector>
#include <memory>

#define GLFW_INCLUDE_VULKAN

class GLFWContext {
public:
    GLFWContext();
    ~GLFWContext();
    std::vector<const char*> getRequiredInstanceExtensions() const;
    static std::shared_ptr<GLFWContext> createSharedInstance();
};

#endif