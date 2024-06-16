#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>
#include <iostream>
#include <cstring>

struct VgeRendererQueueFamilies
{
    int graphicsId = -1;
    int presentId = -1;

    bool IsComplete() {
        return graphicsId != -1 && presentId != -1;
    }
};

struct VgeRendererVkInitCtx
{
    GLFWwindow* window;
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugger;
    VkSurfaceKHR surface;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VgeRendererQueueFamilies queueFamilies;
};

class VgeRendererVkInit
{
public:
    VgeRendererVkInit(GLFWwindow* window);
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

    void createSurface();
    void destroySurface();

    void selectPhysicalDevice();

    static VgeRendererQueueFamilies findQueueFamily(VgeRendererVkInitCtx& ctx, VkPhysicalDevice device);
    static bool IsDeviceUsable(VgeRendererVkInitCtx& ctx, VkPhysicalDevice device);
    static std::vector<const char*> GetRequiredExts();
    static void FillDebuggerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& info);
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData
    );
};