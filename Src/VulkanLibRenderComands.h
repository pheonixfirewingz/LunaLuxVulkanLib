//
// Created by luket on 18/01/2021.
//

#ifndef GITIGNORE_VULKANLIBRENDERCOMANDS_H
#define GITIGNORE_VULKANLIBRENDERCOMANDS_H
//TODO: write documentation
#include <vulkan/vulkan_core.h>
namespace LunaLuxVulkanLib
{
    VkCommandPool vkGenCommandPool(VkCommandPoolCreateFlags flags);

    VkRenderPassBeginInfo vkClearColour(float,float,float,float);

    void vkQueueWaitIdle();
}
#endif //GITIGNORE_VULKANLIBRENDERCOMANDS_H
