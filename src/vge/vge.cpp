#include "vge.hpp"

Vge::Vge(int width, int height, const std::string &title, const std::string &iconPath, bool fullscreen)
{
    try {
        VgeWindowInfo info = {};
        info.fullscreen = fullscreen;
        info.width = width;
        info.height = height;
        info.iconPath = iconPath;
        info.title = title;

        window = new VgeWindow(&info);
    } catch (std::exception& e) {
        std::cerr << "VGE :- " << e.what() << std::endl;
        std::cin.get();
        std::exit(EXIT_FAILURE);
    }
}

Vge::~Vge()
{
    delete window;
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
    window->Update();
}

void Vge::Render()
{

}