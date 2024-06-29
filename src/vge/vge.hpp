#pragma once

#include "window/vge_window.hpp"
#include "renderer/vge_renderer.hpp"

#include <stdexcept>
#include <iostream>
#include <memory>

#define VGE_STD_WINDOW_WIDTH     800
#define VGE_STD_WINDOW_HEIGHT    600

class Vge
{
public:
    Vge(int width, int height, const std::string &iconPath, bool fullscreen);

    void Run();
private:
    std::shared_ptr<VgeWindow> window;
    std::shared_ptr<VgeRenderer> renderer;

    void Update();
    void Render();
};