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
    VkFence vkCreateFence();
    void vkQueueWaitIdle();
    VkRenderPassBeginInfo vkClearColour(float,float,float);
    void vkSetViewport(VkCommandBuffer,float,float);
    void vkSetScissor(VkCommandBuffer,float,float);
}
using namespace LunaLuxVulkanLib;
#endif //GITIGNORE_VULKANLIBRENDERCOMANDS_H
