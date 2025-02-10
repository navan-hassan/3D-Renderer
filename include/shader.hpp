#ifndef SHADER_HPP
#define SHADER_HPP

#include <fstream>
#include <vector>
#include <vulkan_context.hpp>
#include <device.hpp>

static std::vector<char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

static std::vector<char> readFile(const char* filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

enum ShaderType {
    VERTEX,
    FRAGMENT,
    UNKOWN
};

class ShaderModule {
public:
    ShaderModule(VkShaderStageFlagBits shaderStage, const std::vector<char>& shaderCode, std::shared_ptr<VulkanContext> pVkContext, std::shared_ptr<LogicalDevice> pLogicalDevice)
    : stage(shaderStage), pVkContext(pVkContext), pLogicalDevice(pLogicalDevice){
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = shaderCode.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

        const VkAllocationCallbacks* pAllocationCallbacks = pVkContext->getAllocationCallbacks();
        if (vkCreateShaderModule(pLogicalDevice->getHandle(), &createInfo, pAllocationCallbacks, &shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module!");
        }

        debug_write("Successfully created shader module");
    }

    ~ShaderModule() {
        if (shaderModule == VK_NULL_HANDLE) return;

        VkDevice logicalDevice = pLogicalDevice->getHandle();
        const VkAllocationCallbacks* pAllocationCallbacks = pVkContext->getAllocationCallbacks();

        vkDestroyShaderModule(logicalDevice, shaderModule, pAllocationCallbacks);
        debug_write("Successfully destroyed shader module");
    }

    void createPipelineStage() {
        if (shaderModule == VK_NULL_HANDLE) return;

        shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageInfo.stage = stage;
        shaderStageInfo.module = shaderModule;
        shaderStageInfo.pName = "main";

    }

    VkShaderModule getHandle() const {
        return this->shaderModule;
    }

    VkPipelineShaderStageCreateInfo getStageInfo() const {
        return shaderStageInfo;
    }

private:
    VkShaderStageFlagBits stage;
    VkShaderModule shaderModule = VK_NULL_HANDLE;
    VkPipelineShaderStageCreateInfo shaderStageInfo{};
    std::shared_ptr<VulkanContext> pVkContext;
    std::shared_ptr<LogicalDevice> pLogicalDevice;
};

#endif