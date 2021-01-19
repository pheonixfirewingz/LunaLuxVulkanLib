//
// Created by luket on 19/01/2021.
//
#include "VulkanLibRenderComands.h"
namespace LunaLuxVulkanLib
{
    void vkSetViewport(VkCommandBuffer buffer,float width, float height)
    {
        VkViewport viewport = {};
        viewport.width = width;
        viewport.height = height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(buffer,0,1,&viewport);
    }
    
    void vkSetScissor(VkCommandBuffer buffer, float width, float height)
    {
        VkRect2D rect = {};
        rect.offset = {0,0};
        rect.extent.width = width;
        rect.extent.height= height;
        vkCmdSetScissor(buffer,0,1,&rect);
    }
}
