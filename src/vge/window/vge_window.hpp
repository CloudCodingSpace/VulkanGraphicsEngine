#pragma once

#include <stb/stb_image.h>
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <string>

struct VgeWindowInfo
{
    int width, height;
    std::string title;
    bool fullscreen;
    std::string iconPath;
};

class VgeWindow
{
public:
    VgeWindow(VgeWindowInfo* info);
    ~VgeWindow();

    void Show();
    void Update();
    bool IsCloseButtonPressed() { return glfwWindowShouldClose(window); }

private:
    VgeWindowInfo *info;
    GLFWwindow *window;

    void createWindow();
};