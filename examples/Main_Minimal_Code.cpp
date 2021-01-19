// Created by luket on 31/12/2020.
#include <LunaLuxWindowLib/Window.h>
#include "VulkanLib.h"
#include "VulkanLibRenderComands.h"
VkFence	fence = nullptr;
int main()
{
    auto* window = new LunaLuxWindowLib::Window();
    window->Open("Vulkan Library Test 2",NULL,NULL);
    createContext(false,window);

    VkCommandPool command_pool = vkGenCommandPool(VK_COMMAND_POOL_CREATE_TRANSIENT_BIT |
                                                  VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    fence = vkCreateFence();

    VkCommandBuffer command_buffer					= VK_NULL_HANDLE;
    VkCommandBufferAllocateInfo	command_buffer_allocate_info {};
    command_buffer_allocate_info.sType				= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    command_buffer_allocate_info.commandPool		= command_pool;
    command_buffer_allocate_info.level				= VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    command_buffer_allocate_info.commandBufferCount	= 1;
    vkAllocateCommandBuffers( getDevice(), &command_buffer_allocate_info, &command_buffer );

    VkSemaphore render_complete_semaphore	= VK_NULL_HANDLE;
    VkSemaphoreCreateInfo semaphore_create_info {};
    semaphore_create_info.sType				= VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    vkCreateSemaphore( getDevice(), &semaphore_create_info, nullptr, &render_complete_semaphore );

    while (!window->ShouldClose())
    {
        window->Update(30.0);
        updateContext(window);
        frameBegin(fence);

        VkCommandBufferBeginInfo command_buffer_begin_info{};
        command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        command_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info);
        VkRenderPassBeginInfo render_pass_begin_info = vkClearColour(0.0f, 0.0f, 1.0f);
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
        frameSubmit({render_complete_semaphore}, submit_info);
    }
    vkQueueWaitIdle();
    vkDestroyFence(getDevice(),fence, nullptr);
    vkDestroySemaphore( getDevice(), render_complete_semaphore, nullptr );
    vkDestroyCommandPool( getDevice(), command_pool, nullptr );
    destroyContext();
    return 0;
}