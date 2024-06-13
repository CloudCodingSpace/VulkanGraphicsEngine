#include "vge_renderer.hpp"

VgeRenderer::VgeRenderer()
{
    init = new VgeRendererVkInit();
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