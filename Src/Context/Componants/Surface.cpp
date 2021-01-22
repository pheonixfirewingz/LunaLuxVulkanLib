//
// Created by luket on 22/01/2021.
//

//TODO: write documentation
#include "Surface.h"
#include "../Interface/Platform.h"
#include <stdexcept>
#ifdef LLE_WINDOWS
#include <windows.h>
#include <vulkan/vulkan_win32.h>
#include <vector>

#endif
namespace LunaLuxVulkanLib
{
    Surface::Surface(VkInstance instance,Device* device,LunaLuxWindowLib::Window* window) : instance(instance)
    {
#ifdef LLE_WINDOWS
        VkWin32SurfaceCreateInfoKHR surface_info = {};
            surface_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
            surface_info.hinstance = GetModuleHandle(nullptr);
            surface_info.hwnd = (HWND) window->GetNativeWindow();

            vkCreateWin32SurfaceKHR(instance, &surface_info, nullptr, &surface);
#endif
        VkBool32 support{false};
        vkGetPhysicalDeviceSurfaceSupportKHR(device->getPDev(),device->getGraphicFamilyIndex(),surface,&support);
        if(!support) throw std::runtime_error("something is not right with surface creation");
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device->getPDev(),surface,&surfaceCapabilities);

        uint32_t count = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device->getPDev(), surface, &count, nullptr);
        if (count == 0) throw std::runtime_error("no surface formats found");
        std::vector<VkSurfaceFormatKHR> formats(count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device->getPDev(), surface, &count, formats.data());
        if (formats[0].format == VK_FORMAT_UNDEFINED)
        {
            surfaceFormat.format = VK_FORMAT_B8G8R8A8_UNORM;
            surfaceFormat.colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
        }
        else surfaceFormat = formats[0];
    }

    Surface::~Surface()
    {
        vkDestroySurfaceKHR(instance,surface, nullptr);
    }

    const VkSurfaceKHR Surface::getSurface() const
    {
        return surface;
    }

    const VkSurfaceCapabilitiesKHR Surface::getSurfaceCapabilities() const
    {
        return surfaceCapabilities;
    }

    const VkSurfaceFormatKHR Surface::getSurfaceFormat() const
    {
        return surfaceFormat;
    }

};
