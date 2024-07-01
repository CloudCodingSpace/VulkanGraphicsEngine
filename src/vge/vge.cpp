#include "vge.hpp"

Vge::Vge(int width, int height, const std::string &iconPath, bool fullscreen)
{
    try {
        VgeWindowInfo info = {};
        info.fullscreen = fullscreen;
        info.width = width;
        info.height = height;
        info.iconPath = iconPath;
        info.title = "VulkanGraphicsEngine";

        window = std::make_shared<VgeWindow>(&info);

        renderer = std::make_shared<VgeRenderer>(window->GetWindowHND());
    } catch (std::exception& e) {
        std::cerr << "VGE :- " << e.what() << std::endl;
        std::cin.get();
        std::exit(EXIT_FAILURE);
    }
}

void Vge::Run()
{
    window->Show();

    bool running = true;
    while(running) {
        Update();
        Render();

        if(window->IsCloseButtonPressed()) {
            running = false;
        }
    }
}

void Vge::Update()
{
    renderer->Update();
    window->Update();
}

void Vge::Render()
{
    renderer->Render();
}