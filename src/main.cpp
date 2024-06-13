#include "vge/vge.hpp"

int main(int argc, const char** argv) {
    Vge* vge = new Vge(VGE_STD_WINDOW_WIDTH, VGE_STD_WINDOW_HEIGHT, "assets/icons/vk.png", false);    
    vge->Run();
    delete vge;

    std::cin.get();
    return EXIT_SUCCESS;
}