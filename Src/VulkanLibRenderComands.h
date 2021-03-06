// Created by luket on 18/01/2021.
#ifndef GITIGNORE_VULKANLIBRENDERCOMANDS_H
#define GITIGNORE_VULKANLIBRENDERCOMANDS_H
//TODO: write documentation
#include <vulkan/vulkan_core.h>
#include <LunaLuxWindowLib/Window.h>
#include <vector>
namespace LunaLuxVulkanLib
{
    //command pool
    //-----------------------------------------------------------------------------------------------------------------
    VkCommandPool vkGenCommandPool(VkCommandPoolCreateFlags flags);
    VkCommandBuffer vkAllocateCommandBuffers(VkCommandBufferAllocateInfo*);
    void vkDestroyCommandPool(VkCommandPool);

    //threads
    //-----------------------------------------------------------------------------------------------------------------
    VkFence vkGenFence();
    VkSemaphore vkGenSemaphore();
    void vkQueueWaitIdle();
    void vkDestroyFence(VkFence);
    void vkDestroySemaphore(VkSemaphore);

    //descriptor
    //-----------------------------------------------------------------------------------------------------------------
    void vkGenUniformDescriptorPool(VkDescriptorPool&,uint32_t,uint32_t);

    //pipeline
    //-----------------------------------------------------------------------------------------------------------------
    std::tuple<VkPipeline,VkPipelineLayout> vkGenDefaultPipeline(LunaLuxWindowLib::Window*,VkPipelineShaderStageCreateInfo[],VkPipelineVertexInputStateCreateInfo,
                                    VkPipelineInputAssemblyStateCreateInfo,uint32_t,VkDescriptorSetLayout*);
    VkShaderModule vkGenShaderModule(const std::vector<char>&);
    VkPipelineShaderStageCreateInfo vkGenShaderLink(VkShaderModule,VkShaderStageFlagBits);
    void vkDestroyShaderModule(VkShaderModule);
    void vkDestroyPipeline(VkPipeline);
    void vkDestroyPipelineLayout(VkPipelineLayout);

    //command buffer commands
    //-----------------------------------------------------------------------------------------------------------------
    VkRenderPassBeginInfo vkClearColour(const float,const float,const float);
    void vkSetViewport(VkCommandBuffer,float,float);
    void vkSetScissor(VkCommandBuffer,float,float);

    //buffer commands
    //-----------------------------------------------------------------------------------------------------------------
    void vkGenBuffer(void* data_in, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                     VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    [[maybe_unused]] VkDeviceMemory vkGetUniformBufferMemory(int32_t);
    //this best used for updating your uniform shader buffer data;
    void vkBufferUpdateData(void * data_in, VkDeviceMemory BufferMemory,uint64_t size);
    //retrieves internal uniform buffers
    std::vector<VkBuffer> vkGetUnifromBuffers();
    //this is a helper function to help transfer data from on buffer to another
    [[maybe_unused]] void vkCopyBuffer(VkCommandPool commandPool,VkBuffer srcBuffer,
                                                     VkBuffer dstBuffer, VkDeviceSize size);
    void vkDestroyBuffer(VkBuffer,VkDeviceMemory);
}
using namespace LunaLuxVulkanLib;
#endif //GITIGNORE_VULKANLIBRENDERCOMANDS_H
