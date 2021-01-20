//
// Created by luket on 31/12/2020.
//
#include <LunaLuxWindowLib/Window.h>
#include <stdexcept>
#include <fstream>
#include "VulkanLib.h"
#include "VulkanLibRenderComands.h"
static std::vector<char> readFile(const std::string& filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) throw std::runtime_error("failed to open file!");

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

int main()
{
    VkFence	fence = nullptr;
    auto* window = new LunaLuxWindowLib::Window();
    window->Open("Vulkan Library Test 2",NULL,NULL);
    createContext(false,window);

    VkCommandPool command_pool = vkGenCommandPool(VK_COMMAND_POOL_CREATE_TRANSIENT_BIT |
                                                       VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    fence = vkGenFence();

    VkCommandBufferAllocateInfo	command_buffer_allocate_info {};
    command_buffer_allocate_info.sType				= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    command_buffer_allocate_info.commandPool		= command_pool;
    command_buffer_allocate_info.level				= VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    command_buffer_allocate_info.commandBufferCount	= 1;
    VkCommandBuffer command_buffer = vkAllocateCommandBuffers(&command_buffer_allocate_info);

    VkSemaphore render_complete_semaphore = vkGenSemaphore();

    VkShaderModule vertShaderModule = vkGenShaderModule(readFile("vert.spv"));
    VkShaderModule fragShaderModule = vkGenShaderModule(readFile("frag.spv"));

    VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    auto[graphicsPipelines,pipelineLayout] = vkGenDefaultPipeline(window,shaderStages,vertexInputInfo,inputAssembly);

    vkDestroyShaderModule(vertShaderModule);
    vkDestroyShaderModule(fragShaderModule);


    while (!window->ShouldClose())
    {
        window->Update(30.0);
        updateContext(window);
        frameBegin(fence);

        VkCommandBufferBeginInfo command_buffer_begin_info{};
        command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        command_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info);
        VkRenderPassBeginInfo render_pass_begin_info = vkClearColour(0.5f, 0.5f, 0.5f);
        vkCmdBeginRenderPass(command_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

        auto[width,height]= window->GetWindowSize();
        vkSetViewport(command_buffer,(float)width,(float)height);
        vkSetScissor(command_buffer,(float)width,(float)height);

        vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipelines);
        vkCmdDraw(command_buffer, 3, 1, 0, 0);

        vkCmdEndRenderPass(command_buffer);
        vkEndCommandBuffer(command_buffer);

        VkSubmitInfo submit_info{};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.waitSemaphoreCount = 0;
        submit_info.pWaitSemaphores = nullptr;
        submit_info.pWaitDstStageMask = nullptr;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &command_buffer;
        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = &render_complete_semaphore;
        frameSubmit({render_complete_semaphore}, submit_info);
    }
    vkQueueWaitIdle();
    vkDestroyPipeline(graphicsPipelines);
    vkDestroyPipelineLayout(pipelineLayout);
    vkDestroyFence(fence);
    vkDestroySemaphore(render_complete_semaphore);
    vkDestroyCommandPool(command_pool);
    destroyContext();
    return 0;
}