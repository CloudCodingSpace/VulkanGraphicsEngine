#include "vge_renderer.hpp"

VgeRenderer::VgeRenderer(std::shared_ptr<VgeWindow>& window)
{
    init = std::make_shared<VgeRendererVkInit>(window->GetWindowHND());
}

void VgeRenderer::Render()
{

}

void VgeRenderer::Update()
{

}