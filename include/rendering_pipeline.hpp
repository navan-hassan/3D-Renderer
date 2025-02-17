#ifndef RENDERING_PIPELINE_HPP
#define RENDERING_PIPELINE_HPP

#include <memory>
#include <vulkan_context.hpp>
#include <shader.hpp>
#include <constants.hpp>

class RenderingPipeline {
public:
    RenderingPipeline(std::shared_ptr<VulkanContext> pVulkanContext, std::shared_ptr<LogicalDevice> pLogicalDevice);
    void build();
private:
    std::shared_ptr<VulkanContext> pVulkanContext;
    std::shared_ptr<ShaderModule> pVertexModule;
    std::shared_ptr<ShaderModule> pFragmentModule;
    std::shared_ptr<LogicalDevice> pLogicalDevice;
};
#endif