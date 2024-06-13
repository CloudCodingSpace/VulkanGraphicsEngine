#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>
#include <iostream>
#include <cstring>

struct VgeRendererVkInitCtx
{
    const GLFWwindow* window;
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugger;
};

class VgeRendererVkInit
{
public:
    VgeRendererVkInit();
    ~VgeRendererVkInit();

    const VgeRendererVkInitCtx* GetCtx() { return reinterpret_cast<const VgeRendererVkInitCtx*>(&ctx); }
private:
    VgeRendererVkInitCtx ctx;
#ifdef VGE_DEBUG
    static const bool debug = true;
#else
    static const bool debug = false;
#endif

    const std::vector<const char*> layers = {
        "VK_LAYER_KHRONOS_validation"
    };

    void createInstance();
    void destroyInstance();

    bool checkDebuggerSupport();
    void createDebugger();
    void destroyDebugger();

    static std::vector<const char*> GetRequiredExts();
    static void FillDebuggerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& info);
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData
    );
};