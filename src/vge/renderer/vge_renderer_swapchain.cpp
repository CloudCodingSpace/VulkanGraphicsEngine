#include "vge_renderer_swapchain.hpp"

VgeRendererSwapchain::VgeRendererSwapchain(std::shared_ptr<VgeRendererVkInit> &init, GLFWwindow* window)
{
    u_Init = init->GetCtx();
    u_Window = window;
    m_Details = init->GetScSupportDetails();

    CreateSwapchain();
    GetScImages();
    CreateImageViews();
    CreateRenderPass();
    CreateFrameBuffers();
}

VgeRendererSwapchain::~VgeRendererSwapchain()
{
    Cleanup();
    vkDestroyRenderPass(u_Init->device, m_Pass, nullptr);
}

void VgeRendererSwapchain::Recreate()
{
    int width, height;
    glfwGetFramebufferSize(u_Window, &width, &height);
    while(width == 0 || height == 0)
    {
        glfwGetFramebufferSize(u_Window, &width, &height);
        glfwWaitEvents();
    }

    CreateSwapchain();
    GetScImages();
    CreateImageViews();
    CreateFrameBuffers();
}

void VgeRendererSwapchain::CreateSwapchain()
{
    m_ScExtent = SelectExtent(m_Details.caps);
    m_Mode = SelectPresentMode(m_Details.modes);
    auto format = SelectSurfaceFormat(m_Details.formats);

    m_ScFormat = format.format;

    uint32_t imgCount = m_Details.caps.minImageCount + 1;
    if (m_Details.caps.maxImageCount > 0 && imgCount > m_Details.caps.maxImageCount) {
        imgCount = m_Details.caps.maxImageCount;
    }

    VkSwapchainCreateInfoKHR info{};
    info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    info.surface = u_Init->surface;
    info.oldSwapchain = VK_NULL_HANDLE;
    info.clipped = VK_TRUE;
    info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    info.imageExtent = m_ScExtent;
    info.imageFormat = format.format;
    info.imageColorSpace = format.colorSpace;
    info.presentMode = m_Mode;
    info.minImageCount = imgCount;
    info.imageArrayLayers = 1;
    info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    info.preTransform = m_Details.caps.currentTransform;

    if(u_Init->queueFamilies.graphicsId != u_Init->queueFamilies.presentId)
    {
        uint32_t indices[] = { 
            static_cast<uint32_t>(u_Init->queueFamilies.graphicsId), 
            static_cast<uint32_t>(u_Init->queueFamilies.presentId)
        };

        info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        info.queueFamilyIndexCount = 2;
        info.pQueueFamilyIndices = indices;
    } 
    else
    {
        info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    if(vkCreateSwapchainKHR(u_Init->device, &info, nullptr, &m_Hnd) != VK_SUCCESS)
        throw std::runtime_error("Failed to create the swap chain!");
}

void VgeRendererSwapchain::GetScImages()
{
    uint32_t imgCount = 0;
    vkGetSwapchainImagesKHR(u_Init->device, m_Hnd, &imgCount, nullptr);
    m_ScImgs.resize(imgCount);
    vkGetSwapchainImagesKHR(u_Init->device, m_Hnd, &imgCount, m_ScImgs.data());
}

void VgeRendererSwapchain::CreateRenderPass()
{
    VkAttachmentDescription attachment{};
    attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    attachment.format = m_ScFormat;
    attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

    VkAttachmentReference reference{};
    reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    reference.attachment = 0;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &reference;

    VkRenderPassCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    info.attachmentCount = 1;
    info.pAttachments = &attachment;
    info.subpassCount = 1;
    info.pSubpasses = &subpass;
    info.dependencyCount = 1;
    info.pDependencies = &dependency;

    if(vkCreateRenderPass(u_Init->device, &info, nullptr, &m_Pass) != VK_SUCCESS) 
        throw std::runtime_error("Failed to create the render pass!"); 
}

void VgeRendererSwapchain::CreateImageViews()
{
    m_ScImgViews.resize(m_ScImgs.size());
    for(size_t i = 0; i < m_ScImgs.size(); i++) {
        VkImageViewCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        info.format = m_ScFormat;
        info.image = m_ScImgs[i];
        info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        info.subresourceRange.layerCount = 1;
        info.subresourceRange.levelCount = 1;
        info.subresourceRange.baseMipLevel = 0;
        info.subresourceRange.baseArrayLayer = 0;
        info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

        if(vkCreateImageView(u_Init->device, &info, nullptr, &m_ScImgViews[i]) != VK_SUCCESS) 
            throw std::runtime_error("Failed to create the swap chain image view!");
    }
}

void VgeRendererSwapchain::CreateFrameBuffers()
{
    m_ScFrameBuffs.resize(m_ScImgs.size());
    for(size_t i = 0; i < m_ScImgs.size(); i++) 
    {
        VkImageView views[] = { m_ScImgViews[i] };

        VkFramebufferCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        info.attachmentCount = 1;
        info.pAttachments = views;
        info.renderPass = m_Pass;
        info.width = m_ScExtent.width;
        info.height = m_ScExtent.height;
        info.layers = 1;

        if(vkCreateFramebuffer(u_Init->device, &info, nullptr, &m_ScFrameBuffs[i]) != VK_SUCCESS)
            throw std::runtime_error("Failed to create the framebuffers!");
    }
}

void VgeRendererSwapchain::Cleanup()
{
    for(const auto& buff : m_ScFrameBuffs) 
    {
        vkDestroyFramebuffer(u_Init->device, buff, nullptr);
    }

    for(const auto& view : m_ScImgViews)
    {
        vkDestroyImageView(u_Init->device, view, nullptr);
    }

    vkDestroySwapchainKHR(u_Init->device, m_Hnd, nullptr);
}

VkSurfaceFormatKHR VgeRendererSwapchain::SelectSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats) {
    for (const auto& format : formats) 
    {
        if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            return format;
    }

    return formats[0];
}

VkPresentModeKHR VgeRendererSwapchain::SelectPresentMode(const std::vector<VkPresentModeKHR> &modes)
{
    for(const auto& mode : modes)
    {
        if(mode == VK_PRESENT_MODE_MAILBOX_KHR)
            return mode;
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VgeRendererSwapchain::SelectExtent(const VkSurfaceCapabilitiesKHR &caps)
{
    if(caps.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return caps.currentExtent;
    }
    else 
    {
        int width, height;
        glfwGetFramebufferSize(u_Window, &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(actualExtent.width, caps.minImageExtent.width, caps.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, caps.minImageExtent.height, caps.maxImageExtent.height);

        return actualExtent;
    }
}