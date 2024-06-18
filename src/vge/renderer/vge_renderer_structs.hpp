#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include <vector>

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
    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VgeRendererQueueFamilies queueFamilies;
};

struct VgeRendererVkScSupportDetails
{
    VkSurfaceCapabilitiesKHR caps;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> modes;

    bool IsCompleted() {
        return !formats.empty() && !modes.empty();
    }
};