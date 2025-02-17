#include <rendering_pipeline.hpp>
#include <io_utilities.hpp>

RenderingPipeline::RenderingPipeline(std::shared_ptr<VulkanContext> pVulkanContext, std::shared_ptr<LogicalDevice> pLogicalDevice)
    : pVulkanContext(pVulkanContext), pLogicalDevice(pLogicalDevice)
{
    auto vertShaderCode = readFile(constants::VERTEX_SHADER_PATH);
    auto fragShaderCode = readFile(constants::FRAGMENT_SHADER_PATH);

    pVertexModule = std::shared_ptr<ShaderModule>(new ShaderModule(VK_SHADER_STAGE_VERTEX_BIT, vertShaderCode, pVulkanContext, pLogicalDevice));
    pFragmentModule = std::shared_ptr<ShaderModule>(new ShaderModule(VK_SHADER_STAGE_FRAGMENT_BIT, fragShaderCode, pVulkanContext, pLogicalDevice));
}
void RenderingPipeline::build() {
    VkPipelineShaderStageCreateInfo vertShaderStageInfo = pVertexModule->getStageInfo();
    VkPipelineShaderStageCreateInfo fragShaderStageInfo = pFragmentModule->getStageInfo();
    VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState colorBlending{};
}