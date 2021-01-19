//
// Created by luket on 09/01/2021.
//

#ifndef GITIGNORE_GPU_H
#define GITIGNORE_GPU_H
//TODO: write documentation
#include <vector>
#include <LunaLuxWindowLib/Window.h>
#include <vulkan/vulkan_core.h>
namespace LunaLuxVulkanLib
{
    std::vector<VkQueueFamilyProperties> get_vector_list(VkPhysicalDevice);
    VkPhysicalDevice getPDevice(VkInstance);
    void createWin32Surf(bool,VkInstance, VkSurfaceKHR *,LunaLuxWindowLib::Window*);
    void llegetSurfaceFormat(VkPhysicalDevice,VkSurfaceKHR,VkSurfaceFormatKHR*);
}
#endif //GITIGNORE_GPU_H
