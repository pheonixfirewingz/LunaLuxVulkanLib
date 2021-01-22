//
// Created by luket on 18/01/2021.
//

#ifndef GITIGNORE_VULKANLIBRENDERCOMANDS_H
#define GITIGNORE_VULKANLIBRENDERCOMANDS_H
//TODO: write documentation
#include <vulkan/vulkan_core.h>
#include <LunaLuxWindowLib/Window.h>
#include <vector>
namespace LunaLuxVulkanLib
{
    //command pool
    VkCommandPool vkGenCommandPool(VkCommandPoolCreateFlags flags);
    VkCommandBuffer vkAllocateCommandBuffers(VkCommandBufferAllocateInfo*);
    void vkDestroyCommandPool(VkCommandPool);

    //threads
    VkFence vkGenFence();
    VkSemaphore vkGenSemaphore();
    void vkQueueWaitIdle();
    void vkDestroyFence(VkFence);
    void vkDestroySemaphore(VkSemaphore);

    //pipeline
    std::tuple<VkPipeline,VkPipelineLayout> vkGenDefaultPipeline(LunaLuxWindowLib::Window*,VkPipelineShaderStageCreateInfo[],VkPipelineVertexInputStateCreateInfo,
                                    VkPipelineInputAssemblyStateCreateInfo);
    VkShaderModule vkGenShaderModule(const std::vector<char>&);
    VkPipelineShaderStageCreateInfo vkGenShaderLink(VkShaderModule,VkShaderStageFlagBits);
    void vkDestroyShaderModule(VkShaderModule);
    void vkDestroyPipeline(VkPipeline);
    void vkDestroyPipelineLayout(VkPipelineLayout);

    //command buffer commands
    VkRenderPassBeginInfo vkClearColour(float,float,float);
    void vkSetViewport(VkCommandBuffer,float,float);
    void vkSetScissor(VkCommandBuffer,float,float);

    //buffer commands
    VkBuffer vkGenBuffer(VkBufferCreateInfo*);
    VkMemoryRequirements vkGetBufferMemoryRequirements(VkBuffer);
    VkDeviceMemory vkAllocateMemory(VkMemoryRequirements, VkMemoryPropertyFlags);
    void vkBufferTransferData(void*,VkDeviceMemory,VkBufferCreateInfo);
    void vkFreeMemory(VkDeviceMemory);
    void vkDestroyBuffer(VkBuffer);
}
using namespace LunaLuxVulkanLib;
#endif //GITIGNORE_VULKANLIBRENDERCOMANDS_H
