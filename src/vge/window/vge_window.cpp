#include "vge_window.hpp"

VgeWindow::VgeWindow(VgeWindowInfo *info)
{
    this->info = info;
    createWindow();
}

VgeWindow::~VgeWindow()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void VgeWindow::Show()
{
    glfwShowWindow(window);
}

void VgeWindow::Update()
{
    glfwPollEvents();
}

void VgeWindow::createWindow()
{
    if(!glfwInit())
        throw std::runtime_error("Failed to initialize GLFW!");
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    window = glfwCreateWindow(info->width, info->height, info->title.c_str(), info->fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
    if(window == nullptr)
        throw std::runtime_error("Failed to create window!");
    glfwSetWindowUserPointer(window, this);

    GLFWimage image;
    int channels;
    image.pixels = stbi_load(info->iconPath.c_str(), &image.width, &image.height, &channels, 4);
    glfwSetWindowIcon(window, 1, &image);
}