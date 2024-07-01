#pragma once

#include <vector>
#include <cstdint>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <limits>

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "vge_renderer_structs.hpp"
#include "vge_renderer_vk_init.hpp"
#include "../window/vge_window.hpp"

class VgeRendererSwapchain
{
public:
    VgeRendererSwapchain(std::shared_ptr<VgeRendererVkInit>& init, GLFWwindow* window);
    ~VgeRendererSwapchain();

    VkSwapchainKHR GetHandle() { return m_Hnd; }
    VkRenderPass GetPass() { return m_Pass; }
    VkExtent2D GetExtent() { return m_ScExtent; }
    VkFormat GetFormat() { return m_ScFormat; }
    VkPresentModeKHR GetPresentMode() { return m_Mode; }
    std::vector<VkImage> GetImages() { return m_ScImgs; }
    std::vector<VkImageView> GetImageViews() { return m_ScImgViews; }
    const VgeRendererVkScSupportDetails& GetDetails() { return m_Details; }

    void Recreate();
private:
    std::vector<VkImage> m_ScImgs;
    std::vector<VkImageView> m_ScImgViews;
    std::vector<VkFramebuffer> m_ScFrameBuffs;

    VkSwapchainKHR m_Hnd;
    VkRenderPass m_Pass;
    VkExtent2D m_ScExtent;
    VkFormat m_ScFormat;
    VkPresentModeKHR m_Mode;

    const VgeRendererVkInitCtx* u_Init;

    GLFWwindow* u_Window;
    VgeRendererVkScSupportDetails m_Details;
private:
    void CreateSwapchain();
    void GetScImages();
    void CreateRenderPass();
    void CreateImageViews();
    void CreateFrameBuffers();

    void Cleanup();

    VkSurfaceFormatKHR SelectSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
    VkPresentModeKHR SelectPresentMode(const std::vector<VkPresentModeKHR>& modes);
    VkExtent2D SelectExtent(const VkSurfaceCapabilitiesKHR& caps);
};