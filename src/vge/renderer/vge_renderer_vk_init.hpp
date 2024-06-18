#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>
#include <iostream>
#include <cstring>
#include <set>

#include "vge_renderer_structs.hpp"

class VgeRendererVkInit
{
public:
    VgeRendererVkInit(GLFWwindow* window);
    ~VgeRendererVkInit();


    VgeRendererVkScSupportDetails GetScSupportDetails();
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
    const std::vector<const char*> devExts = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    void createInstance();
    void destroyInstance();

    bool checkDebuggerSupport();
    void createDebugger();
    void destroyDebugger();

    void createSurface();
    void destroySurface();

    void selectPhysicalDevice();
    void createLogicalDevice();
    void getRequiredQueues();
    void destroyLogicalDevice();

    bool checkDeviceExtensions(VgeRendererVkInitCtx& ctx, VkPhysicalDevice device);
    VgeRendererVkScSupportDetails getScSupportDetails(VkPhysicalDevice device);
    VgeRendererQueueFamilies findQueueFamily(VgeRendererVkInitCtx& ctx, VkPhysicalDevice device);
    bool IsDeviceUsable(VgeRendererVkInitCtx& ctx, VkPhysicalDevice device);
    std::vector<const char*> GetRequiredExts();
    void FillDebuggerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& info);
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData
    );
};