//
// Created by luket on 31/12/2020.
//
#include <LunaLuxWindowLib/Window.h>
#include "VulkanLib.h"
#include "VulkanLibRenderComands.h"
VkFence	fence = nullptr;
int main()
{
    auto* window = new LunaLuxWindowLib::Window();
    window->Open("Vulkan Library Test",NULL,NULL);
#ifdef DEBUG
    LunaLuxVulkanLib::createContext(true,window);
#else
    LunaLuxVulkanLib::createContext(false,window);
#endif

    VkCommandPool command_pool = LunaLuxVulkanLib::vkGenCommandPool(VK_COMMAND_POOL_CREATE_TRANSIENT_BIT |
                                                                    VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    VkFenceCreateInfo fenceCreateInfo = {};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.pNext = nullptr;

    vkCreateFence(LunaLuxVulkanLib::getDevice(),&fenceCreateInfo ,nullptr,&fence);

    VkCommandBuffer command_buffer					= VK_NULL_HANDLE;
    VkCommandBufferAllocateInfo	command_buffer_allocate_info {};
    command_buffer_allocate_info.sType				= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    command_buffer_allocate_info.commandPool		= command_pool;
    command_buffer_allocate_info.level				= VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    command_buffer_allocate_info.commandBufferCount	= 1;
    vkAllocateCommandBuffers( LunaLuxVulkanLib::getDevice(), &command_buffer_allocate_info, &command_buffer );

    VkSemaphore render_complete_semaphore	= VK_NULL_HANDLE;
    VkSemaphoreCreateInfo semaphore_create_info {};
    semaphore_create_info.sType				= VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    vkCreateSemaphore( LunaLuxVulkanLib::getDevice(), &semaphore_create_info, nullptr, &render_complete_semaphore );

    while (!window->ShouldClose())
    {
        window->Update(30.0);
            LunaLuxVulkanLib::updateContext(window);
            LunaLuxVulkanLib::frameBegin(fence);

            VkCommandBufferBeginInfo command_buffer_begin_info{};
            command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            command_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

            vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info);

            //TODO: debug why clear colour is not working
            VkRenderPassBeginInfo render_pass_begin_info = LunaLuxVulkanLib::vkClearColour(0.0f, 100.0f, 0.0f, 1.0f);

            vkCmdBeginRenderPass(command_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

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
            LunaLuxVulkanLib::frameSubmit({render_complete_semaphore}, submit_info);
    }
    LunaLuxVulkanLib::vkQueueWaitIdle();
    vkDestroyFence(LunaLuxVulkanLib::getDevice(),fence, nullptr);
    vkDestroySemaphore( LunaLuxVulkanLib::getDevice(), render_complete_semaphore, nullptr );
    vkDestroyCommandPool( LunaLuxVulkanLib::getDevice(), command_pool, nullptr );
    LunaLuxVulkanLib::destroyContext();
    return 0;
}