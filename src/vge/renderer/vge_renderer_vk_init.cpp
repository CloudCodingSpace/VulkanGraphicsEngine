#include "vge_renderer_vk_init.hpp"

VgeRendererVkInit::VgeRendererVkInit(GLFWwindow* window)
{
    ctx.window = window;

    createInstance();
    createDebugger();
    createSurface();
    selectPhysicalDevice();
    createLogicalDevice();
    getRequiredQueues();
}

VgeRendererVkInit::~VgeRendererVkInit()
{
    destroyLogicalDevice();
    destroySurface();
    destroyDebugger();
    destroyInstance();
}

VgeRendererVkScSupportDetails VgeRendererVkInit::GetScSupportDetails()
{
    return getScSupportDetails(ctx.physicalDevice);
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

void VgeRendererVkInit::createSurface()
{
    if(glfwCreateWindowSurface(ctx.instance, ctx.window, nullptr, &ctx.surface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create surface!");
    }
}

void VgeRendererVkInit::destroySurface()
{
    vkDestroySurfaceKHR(ctx.instance, ctx.surface, nullptr);
}

void VgeRendererVkInit::selectPhysicalDevice()
{
    uint32_t count = 0;
    vkEnumeratePhysicalDevices(ctx.instance, &count, nullptr);
    std::vector<VkPhysicalDevice> devices(count);
    vkEnumeratePhysicalDevices(ctx.instance, &count, devices.data());

    for(const auto& device : devices) {
        if(IsDeviceUsable(ctx, device)) {
            ctx.physicalDevice = device;
        }
    }

    if(ctx.physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("Failed to find any suitable device in the current PC!");
    }

    ctx.queueFamilies = findQueueFamily(ctx, ctx.physicalDevice);
}

void VgeRendererVkInit::createLogicalDevice()
{
    float qPriority = 1.0f;

    std::vector<VkDeviceQueueCreateInfo> qCreateInfos;
    std::set<int> queueIds = {ctx.queueFamilies.graphicsId, ctx.queueFamilies.presentId};
    for(const auto& id : queueIds) {
        VkDeviceQueueCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        createInfo.pQueuePriorities = &qPriority;
        createInfo.queueCount = 1;
        createInfo.queueFamilyIndex = id;

        qCreateInfos.push_back(createInfo);
    }

    VkPhysicalDeviceFeatures features{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(qCreateInfos.size());
    createInfo.pQueueCreateInfos = qCreateInfos.data();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(devExts.size());
    createInfo.ppEnabledExtensionNames = devExts.data();
    createInfo.pEnabledFeatures = &features;
    if(debug) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
        createInfo.ppEnabledLayerNames = layers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if(vkCreateDevice(ctx.physicalDevice, &createInfo, nullptr, &ctx.device) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create logical device!");
    }
}

void VgeRendererVkInit::getRequiredQueues()
{
    vkGetDeviceQueue(ctx.device, ctx.queueFamilies.graphicsId, 0, &ctx.graphicsQueue);
    vkGetDeviceQueue(ctx.device, ctx.queueFamilies.presentId, 0, &ctx.presentQueue);
}

void VgeRendererVkInit::destroyLogicalDevice()
{
    vkDestroyDevice(ctx.device, nullptr);
}

bool VgeRendererVkInit::checkDeviceExtensions(VgeRendererVkInitCtx& ctx, VkPhysicalDevice device)
{
    uint32_t count = 0;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &count, nullptr);
    std::vector<VkExtensionProperties> exts(count);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &count, exts.data());

    std::set<std::string> requiredExts(devExts.begin(), devExts.end());

    for(const auto& ext : exts) {
        requiredExts.erase(ext.extensionName);
    }

    return requiredExts.empty();
}

VgeRendererVkScSupportDetails VgeRendererVkInit::getScSupportDetails(VkPhysicalDevice device)
{
    VgeRendererVkScSupportDetails details{};

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, ctx.surface, &details.caps);

    uint32_t formatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, ctx.surface, &formatCount, nullptr);
    details.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, ctx.surface, &formatCount, details.formats.data());

    uint32_t modeCount = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, ctx.surface, &modeCount, nullptr);
    details.modes.resize(modeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, ctx.surface, &modeCount, details.modes.data());

    return details;
}

VgeRendererQueueFamilies VgeRendererVkInit::findQueueFamily(VgeRendererVkInitCtx &ctx, VkPhysicalDevice device)
{
    uint32_t count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);
    std::vector<VkQueueFamilyProperties> props(count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, props.data());

    VgeRendererQueueFamilies families{};

    int i = 0;
    for(const VkQueueFamilyProperties prop : props) {
        VkBool32 presentMode = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, ctx.surface, &presentMode);

        if(prop.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            families.graphicsId = i;
        }

        if(presentMode) {
            families.presentId = i;
        }

        if(families.IsComplete()) {
            break;
        }

        i++;
    }

    return families;
}

bool VgeRendererVkInit::IsDeviceUsable(VgeRendererVkInitCtx& ctx, VkPhysicalDevice device)
{
    return findQueueFamily(ctx, device).IsComplete() && checkDeviceExtensions(ctx, device) && getScSupportDetails(device).IsCompleted();
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