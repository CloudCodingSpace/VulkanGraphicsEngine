#include "vge_renderer.hpp"

VgeRenderer::VgeRenderer(GLFWwindow* window)
{
    init = new VgeRendererVkInit(window);
}

VgeRenderer::~VgeRenderer()
{
    delete init;
}

void VgeRenderer::Render()
{

}

void VgeRenderer::Update()
{

}