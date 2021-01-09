//
// Created by luket on 09/01/2021.
//

//TODO: write documentation
#include "ContextInterface.h"
#include "../Common/Utils.h"
#ifdef LLE_WINDOWS
#include <windows.h>
#include <vulkan/vulkan_win32.h>
#endif

namespace LunaLuxVulkanLib
{

    void ContextInterface::createContext(bool debug, LunaLuxWindowLib::Window * window)
    {
        instextend.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#ifdef LLE_WINDOWS
        instextend.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif
        // initialize the VkApplicationInfo structure
        VkApplicationInfo app_info = {};
        app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        app_info.pNext = nullptr;
        app_info.pApplicationName = "LunaLuxVulkanLib_app";
        app_info.applicationVersion = 1;
        app_info.pEngineName = "LunaLuxVulkanLib";
        app_info.engineVersion = 1;
        app_info.apiVersion = VK_API_VERSION_1_0;

        // initialize the VkInstanceCreateInfo structure
        VkInstanceCreateInfo inst_info = {};
        inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        inst_info.pNext = nullptr;
        inst_info.flags = 0;
        inst_info.pApplicationInfo = &app_info;
        inst_info.enabledExtensionCount = instextend.size();
        inst_info.ppEnabledExtensionNames = instextend.data();
        inst_info.enabledLayerCount = 0;
        inst_info.ppEnabledLayerNames = nullptr;

        debug_wrapper(debug,vkCreateInstance(&inst_info, NULL, &instance))

#ifdef LLE_WINDOWS
        VkWin32SurfaceCreateInfoKHR surface_info = {};
        surface_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        surface_info.hinstance = GetModuleHandle(NULL);
        surface_info.hwnd = (HWND)window->GetNativeWindow();

        debug_wrapper(debug,vkCreateWin32SurfaceKHR(instance, &surface_info, nullptr, &surface))
#endif
    }

    void ContextInterface::destroyContext(bool debug)
    {
        //vkDestroyDevice(device, nullptr);
        vkDestroySurfaceKHR(instance,surface, nullptr);
        vkDestroyInstance(instance, nullptr);
    }
};
