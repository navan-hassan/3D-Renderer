#ifndef SHADER_HPP
#define SHADER_HPP

#include <vector>

#include <vulkan_context.hpp>
#include <device.hpp>

enum ShaderType {
    VERTEX,
    FRAGMENT,
    GEOMETRY,
    COMPUTE,
    UNKOWN
};

class ShaderModule {
public:
    ShaderModule(VkShaderStageFlagBits shaderStage, const std::vector<char>& shaderCode, std::shared_ptr<VulkanContext> pVkContext, std::shared_ptr<LogicalDevice> pLogicalDevice);
    ~ShaderModule();
    void createPipelineStage();
    VkShaderModule getHandle() const;
    VkPipelineShaderStageCreateInfo getStageInfo() const;
private:
    VkShaderStageFlagBits stage;
    VkShaderModule shaderModule = VK_NULL_HANDLE;
    VkPipelineShaderStageCreateInfo shaderStageInfo{};
    std::shared_ptr<VulkanContext> pVkContext;
    std::shared_ptr<LogicalDevice> pLogicalDevice;
};

#endif