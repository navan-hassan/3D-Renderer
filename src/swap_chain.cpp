#include <swap_chain.hpp>
#include <logging_utilities.hpp>

#include <algorithm>

SwapChain::SwapChain(std::shared_ptr<VulkanContext> pVkContext, std::shared_ptr<LogicalDevice> pLogicalDevice)
    : pVkContext(pVkContext), pLogicalDevice(pLogicalDevice) {}

SwapChain::~SwapChain() {

    VkDevice logicaldevice = pLogicalDevice->getHandle();
    const VkAllocationCallbacks* pAllocationCallbacks = pVkContext->getAllocationCallbacks();
    for (auto imageView : swapChainImageViews) {
        vkDestroyImageView(logicaldevice, imageView, pAllocationCallbacks);
    }
    logging::DEBUG("SUCCESSFULLY DESTROYED IMAGE VIEWS");


    if (swapChain == VK_NULL_HANDLE) return;

    vkDestroySwapchainKHR(logicaldevice, swapChain, pAllocationCallbacks);
    logging::DEBUG("SUCCESSFULLY DESTROYED SWAP CHAIN");
}

VkResult SwapChain::init(const SwapChainCreationInfo& creationInfo) {
    VkPhysicalDevice physicalDevice = pLogicalDevice->getPhysicalDevice();

    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    std::vector<VkSurfaceFormatKHR> surfaceFormats;
    std::vector<VkPresentModeKHR> presentModes;
    //VkSurfaceKHR surface = pWindow->getSurface();
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, creationInfo.surface, &surfaceCapabilities);

    uint32_t imageCount = surfaceCapabilities.minImageCount + 1;
    if (surfaceCapabilities.maxImageCount > 0 && imageCount > surfaceCapabilities.maxImageCount) {
        imageCount = surfaceCapabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR vkSwapChainCreateInfo{};
    vkSwapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    vkSwapChainCreateInfo.surface = creationInfo.surface;

    vkSwapChainCreateInfo.minImageCount = imageCount;

    VkSurfaceFormatKHR surfaceFormat = selectSurfaceFormat(physicalDevice, creationInfo.surfaceFormat, creationInfo.surface);
    //FramebufferDimensions framebufferDimensions = pWindow->getFramebufferSize();
    swapChainExtent = setSwapExtent(surfaceCapabilities, static_cast<uint32_t>(creationInfo.framebufferDimensions.width), static_cast<uint32_t>(creationInfo.framebufferDimensions.height), creationInfo.surface, physicalDevice);

    vkSwapChainCreateInfo.imageFormat = surfaceFormat.format;
    vkSwapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
    vkSwapChainCreateInfo.imageExtent = swapChainExtent;
    vkSwapChainCreateInfo.imageArrayLayers = 1;
    vkSwapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndex queueFamilies = pLogicalDevice->getQueueFamilies();

    uint32_t queueFamilyIndices[] = {
        queueFamilies.graphics.value(),
        queueFamilies.presentation.value()
    };

    if (queueFamilies.graphics != queueFamilies.presentation) {
        vkSwapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        vkSwapChainCreateInfo.queueFamilyIndexCount = 2;
        vkSwapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else {
        vkSwapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    vkSwapChainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
    vkSwapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    vkSwapChainCreateInfo.presentMode = (hasRequestedPresentMode(physicalDevice, creationInfo.presentMode, creationInfo.surface))
        ? creationInfo.presentMode : VK_PRESENT_MODE_FIFO_KHR;

    vkSwapChainCreateInfo.clipped = VK_TRUE;
    vkSwapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

    const VkAllocationCallbacks* pAllocationCallbacks = pVkContext->getAllocationCallbacks();
    VkDevice logicaldevice = pLogicalDevice->getHandle();

    if (vkCreateSwapchainKHR(logicaldevice, &vkSwapChainCreateInfo, pAllocationCallbacks, &swapChain) != VK_SUCCESS) {
        logging::ERROR("FAILED TO CREATE SWAP CHAIN");
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    vkGetSwapchainImagesKHR(logicaldevice, swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(logicaldevice, swapChain, &imageCount, swapChainImages.data());

    swapChainImageFormat = surfaceFormat.format;

    logging::DEBUG("SUCCESSFULLY CREATED SWAP CHAIN");
    createImageViews();
    return VK_SUCCESS;
}

void SwapChain::createImageViews() {
    swapChainImageViews.resize(swapChainImages.size());

    for (size_t i = 0; i < swapChainImages.size(); i++) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swapChainImageFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;
        if (vkCreateImageView(pLogicalDevice->getHandle(), &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
            throw std::runtime_error("FAILED TO CREATE IMAGE VIEWS");
        }
    }
    logging::DEBUG("SUCCESSFULLY CREATED IMAGE VIEWS");
}

bool SwapChain::hasRequestedPresentMode(const VkPhysicalDevice& physicalDevice, const VkPresentModeKHR& requestedPresentMode, const VkSurfaceKHR& surface) {
    std::vector<VkPresentModeKHR> presentModes;

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data());
    }

    auto foundPresentMode = std::find_if(
        presentModes.begin(),
        presentModes.end(),
        [requestedPresentMode](const VkPresentModeKHR& vkPresentModeKHR) {
            return vkPresentModeKHR == requestedPresentMode;
        }
    ) != presentModes.end();

    return foundPresentMode;
}

VkSurfaceFormatKHR SwapChain::selectSurfaceFormat(const VkPhysicalDevice& physicalDevice, const VkSurfaceFormatKHR& requestedSurfaceFormat, const VkSurfaceKHR& surface) {
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);

    std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, surfaceFormats.data());

    auto foundSurfaceFormat = std::find_if(
        surfaceFormats.begin(),
        surfaceFormats.end(),
        [requestedSurfaceFormat](const VkSurfaceFormatKHR& vkSurfaceFormat) {
            return vkSurfaceFormat.colorSpace == requestedSurfaceFormat.colorSpace
                && vkSurfaceFormat.format == requestedSurfaceFormat.format;
        }
    ) != surfaceFormats.end();

    if (!foundSurfaceFormat) {
        return surfaceFormats.at(0);
    }

    return requestedSurfaceFormat;
}

VkExtent2D SwapChain::setSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, size_t framebufferWidth, size_t framebufferHeight, const VkSurfaceKHR& surface, const VkPhysicalDevice& physicalDevice) {

    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    }

    VkExtent2D actualExtent = { static_cast<uint32_t>(framebufferWidth), static_cast<uint32_t>(framebufferHeight) };

    actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    return actualExtent;
}
