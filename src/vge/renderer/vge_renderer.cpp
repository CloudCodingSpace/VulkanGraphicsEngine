#include "vge_renderer.hpp"

VgeRenderer::VgeRenderer(GLFWwindow* window)
{
    m_Init = std::make_shared<VgeRendererVkInit>(window);
    m_Swapchain = std::make_shared<VgeRendererSwapchain>(m_Init, window);
}

void VgeRenderer::Render()
{

}

void VgeRenderer::Update()
{

}