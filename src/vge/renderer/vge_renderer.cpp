#include "vge_renderer.hpp"

VgeRenderer::VgeRenderer(std::shared_ptr<VgeWindow>& window)
{
    m_Init = std::make_shared<VgeRendererVkInit>(window->GetWindowHND());
    m_Swapchain = std::make_shared<VgeRendererSwapchain>(m_Init, window);
}

void VgeRenderer::Render()
{

}

void VgeRenderer::Update()
{

}