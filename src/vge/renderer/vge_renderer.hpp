#pragma once

#include <memory>

#include "vge_renderer_vk_init.hpp"
#include "vge_renderer_swapchain.hpp"
#include "../window/vge_window.hpp"

class VgeRenderer
{
public:
    VgeRenderer(GLFWwindow* window);

    void Update();
    void Render();
private:
    std::shared_ptr<VgeRendererVkInit> m_Init;
    std::shared_ptr<VgeRendererSwapchain> m_Swapchain;
};