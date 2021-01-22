//
// Created by luket on 22/01/2021.
//

#ifndef GITIGNORE_SURFACE_H
#define GITIGNORE_SURFACE_H
#include <vulkan/vulkan_core.h>
#include <LunaLuxWindowLib/Window.h>
#include "Device.h"
//TODO: write documentation
namespace LunaLuxVulkanLib
{
    class Surface
    {
    private:
        VkInstance instance = nullptr;
        VkSurfaceKHR surface = nullptr;
        VkSurfaceCapabilitiesKHR surfaceCapabilities;
        VkSurfaceFormatKHR surfaceFormat;
    public:
        Surface(VkInstance,Device*,LunaLuxWindowLib::Window*);
        ~Surface();

        const VkSurfaceKHR getSurface() const;

        const VkSurfaceCapabilitiesKHR getSurfaceCapabilities() const;

        const VkSurfaceFormatKHR getSurfaceFormat() const;
    };
};
#endif //GITIGNORE_SURFACE_H
