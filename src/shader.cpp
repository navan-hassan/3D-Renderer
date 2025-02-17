#include <shader.hpp>
#include <logging_utilities.hpp>

ShaderModule::ShaderModule(VkShaderStageFlagBits shaderStage, const std::vector<char>& shaderCode, std::shared_ptr<VulkanContext> pVkContext, std::shared_ptr<LogicalDevice> pLogicalDevice)
    : stage(shaderStage), pVkContext(pVkContext), pLogicalDevice(pLogicalDevice) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = shaderCode.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

    const VkAllocationCallbacks* pAllocationCallbacks = pVkContext->getAllocationCallbacks();
    if (vkCreateShaderModule(pLogicalDevice->getHandle(), &createInfo, pAllocationCallbacks, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }

    logging::DEBUG("SUCCESSFULLY CREATED SHADER MODULE");
}
ShaderModule::~ShaderModule() {
    if (shaderModule == VK_NULL_HANDLE) return;

    VkDevice logicalDevice = pLogicalDevice->getHandle();
    const VkAllocationCallbacks* pAllocationCallbacks = pVkContext->getAllocationCallbacks();

    vkDestroyShaderModule(logicalDevice, shaderModule, pAllocationCallbacks);
    logging::DEBUG("SUCCESSFULLY DESTROYED SHADER MODULE");
}
void ShaderModule::createPipelineStage() {
    if (shaderModule == VK_NULL_HANDLE) return;

    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageInfo.stage = stage;
    shaderStageInfo.module = shaderModule;
    shaderStageInfo.pName = "main";

}
VkShaderModule ShaderModule::getHandle() const {
    return this->shaderModule;
}
VkPipelineShaderStageCreateInfo ShaderModule::getStageInfo() const {
    return shaderStageInfo;
}