#pragma once

#include <memory>

#include "vge_renderer_vk_init.hpp"
#include "../window/vge_window.hpp"

class VgeRenderer
{
public:
    VgeRenderer(std::shared_ptr<VgeWindow>& window);

    void Update();
    void Render();
private:
    std::shared_ptr<VgeRendererVkInit> init;
};