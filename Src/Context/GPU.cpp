//
// Created by luket on 09/01/2021.
//

#include <stdexcept>
#include "GPU.h"
#include "Interface/Platform.h"
#include "../Common/Utils.h"
//TODO: write documentation
#ifdef LLE_WINDOWS
#include <windows.h>
#include <vulkan/vulkan_win32.h>
#endif
std::vector<VkQueueFamilyProperties> LunaLuxVulkanLib::get_vector_list(VkPhysicalDevice device)
{
    uint32_t count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device,&count, nullptr);
    std::vector<VkQueueFamilyProperties>ret(count);
    vkGetPhysicalDeviceQueueFamilyProperties(device,&count, ret.data());
    return ret;
}

VkPhysicalDevice LunaLuxVulkanLib::getPDevice(VkInstance instance)
{
    uint32_t count = 0;
    vkEnumeratePhysicalDevices(instance, &count, nullptr);
    std::vector<VkPhysicalDevice> pDevice(count);
    vkEnumeratePhysicalDevices(instance, &count, pDevice.data());
    if (count == 1)return pDevice[0];
    else
    {
        throw std::runtime_error("multi gpu support has not been implemented");
    }
}

void LunaLuxVulkanLib::createWin32Surf(bool debug,VkInstance instance, VkSurfaceKHR* surface,LunaLuxWindowLib::Window* window)
{
#ifdef LLE_WINDOWS
            VkWin32SurfaceCreateInfoKHR surface_info = {};
            surface_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
            surface_info.hinstance = GetModuleHandle(nullptr);
            surface_info.hwnd = (HWND) window->GetNativeWindow();

            debug_wrapper(debug, vkCreateWin32SurfaceKHR(instance, &surface_info, nullptr, surface))
#endif
}

void LunaLuxVulkanLib::llegetSurfaceFormat(VkPhysicalDevice p_device, VkSurfaceKHR surface, VkSurfaceFormatKHR * surfaceFormat)
{
    uint32_t count = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(p_device, surface, &count, nullptr);
    if (count == 0) throw std::runtime_error("no surface formats found");
    std::vector<VkSurfaceFormatKHR> formats(count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(p_device, surface, &count, formats.data());
    if (formats[0].format == VK_FORMAT_UNDEFINED)
    {
        surfaceFormat->format = VK_FORMAT_B8G8R8A8_UNORM;
        surfaceFormat->colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    }
    else *surfaceFormat = formats[0];
}
