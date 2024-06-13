#include "vge_renderer_vk_init.hpp"

VgeRendererVkInit::VgeRendererVkInit()
{
    ctx.window = glfwGetCurrentContext();

    createInstance();
    createDebugger();
}

VgeRendererVkInit::~VgeRendererVkInit()
{
    destroyDebugger();
    destroyInstance();
}

void VgeRendererVkInit::createInstance()
{
    if(debug && !checkDebuggerSupport()) {
        throw std::runtime_error("Debug mode but debugger not supported!");
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "VulkanGraphicsEngine";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Vge";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    std::vector<const char*> exts = GetRequiredExts();

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(exts.size());
    createInfo.ppEnabledExtensionNames = exts.data();
    if(debug) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
        createInfo.ppEnabledLayerNames = layers.data();

        VkDebugUtilsMessengerCreateInfoEXT msgInfo{};
        FillDebuggerCreateInfo(msgInfo);

        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &msgInfo;
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if(vkCreateInstance(&createInfo, nullptr, &ctx.instance) != VK_SUCCESS)
        throw std::runtime_error("Failed to create instance!");
}

void VgeRendererVkInit::destroyInstance()
{
    vkDestroyInstance(ctx.instance, nullptr);
}

bool VgeRendererVkInit::checkDebuggerSupport()
{
    uint32_t count = 0;
    vkEnumerateInstanceLayerProperties(&count, nullptr);
    std::vector<VkLayerProperties> props(count);
    vkEnumerateInstanceLayerProperties(&count, props.data());

    for(const auto& layr : layers) {
        for(const auto& prop : props) {
            if(strcmp(layr, prop.layerName) == 0) {
                return true;
            }
        }
    }

    return false;
}

void VgeRendererVkInit::createDebugger()
{
    if(!debug) return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    FillDebuggerCreateInfo(createInfo);

    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(ctx.instance, "vkCreateDebugUtilsMessengerEXT");
    if(func) {
        if(func(ctx.instance, &createInfo, nullptr, &ctx.debugger) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create the debugger!");
        }
    } else {
        throw std::runtime_error("Failed to create the debugger!");
    }
}

void VgeRendererVkInit::destroyDebugger()
{
    if(!debug) return;

    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(ctx.instance, "vkDestroyDebugUtilsMessengerEXT");
    if(func) {
        func(ctx.instance, ctx.debugger, nullptr);
    }
}

std::vector<const char *> VgeRendererVkInit::GetRequiredExts()
{
    uint32_t extCount = 0;
    const char** ext = glfwGetRequiredInstanceExtensions(&extCount);

    std::vector<const char *> exts(ext, extCount + ext);

    if(debug) {
        exts.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return exts;
}

void VgeRendererVkInit::FillDebuggerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &info)
{
    info = {};
    info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
    info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
    info.pfnUserCallback = debugCallback;
}

VKAPI_ATTR VkBool32 VKAPI_CALL VgeRendererVkInit::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData)
{
    std::cerr << pCallbackData->pMessage << std::endl << std::endl;

    return VK_FALSE;
}