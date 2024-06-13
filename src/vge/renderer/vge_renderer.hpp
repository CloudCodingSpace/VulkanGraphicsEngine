#pragma once

#include "vge_renderer_vk_init.hpp"

class VgeRenderer
{
public:
    VgeRenderer();
    ~VgeRenderer();

    void Update();
    void Render();
private:
    VgeRendererVkInit* init;
};