#pragma once

#include "window/vge_window.hpp"
#include "renderer/vge_renderer.hpp"

#include <stdexcept>
#include <iostream>

#define VGE_STD_WINDOW_WIDTH     800
#define VGE_STD_WINDOW_HEIGHT    600

class Vge
{
public:
    Vge(int width, int height, const std::string &iconPath, bool fullscreen);
    ~Vge();

    void Run();
private:
    VgeWindow* window;
    VgeRenderer* renderer;

    void Update();
    void Render();
};