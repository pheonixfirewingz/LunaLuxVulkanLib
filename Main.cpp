//
// Created by luket on 31/12/2020.
//
#include <LunaLuxWindowLib/Window.h>
#include <array>
#include <cmath>
#include "VulkanLib.h"
#include "VulkanLibRenderComands.h"
constexpr float PI				= 3.14159265358979323846;
constexpr float CIRCLE_RAD		= PI * 2;
constexpr float CIRCLE_THIRD_1	= 0;
constexpr float CIRCLE_THIRD_2	= CIRCLE_RAD / 3.0;
constexpr float CIRCLE_THIRD_3	= CIRCLE_THIRD_2 * 2;

int main()
{
    auto* window = new LunaLuxWindowLib::Window();
    window->Open("Vulkan Library Test",NULL,NULL);

    LunaLuxVulkanLib::createContext(true,window);

    VkCommandPool command_pool			= VK_NULL_HANDLE;
    VkCommandPoolCreateInfo pool_create_info {};
    pool_create_info.sType				= VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_create_info.flags				= VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    pool_create_info.queueFamilyIndex	= LunaLuxVulkanLib::getFamilyIndex();
    vkCreateCommandPool(LunaLuxVulkanLib::getDevice(), &pool_create_info, nullptr, &command_pool );

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

    float color_rotator		= 0.0f;
    while (!window->ShouldClose())
    {
        window->Update(30.0);
        LunaLuxVulkanLib::updateContext(window);
        LunaLuxVulkanLib::frameBegin();

        VkCommandBufferBeginInfo command_buffer_begin_info {};
        command_buffer_begin_info.sType				= VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        command_buffer_begin_info.flags				= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer( command_buffer, &command_buffer_begin_info );

        auto[width,height] = window->GetWindowSize();
        VkRect2D render_area {};
        render_area.offset.x		= 0;
        render_area.offset.y		= 0;
        render_area.extent			= {static_cast<uint32_t>(width),static_cast<uint32_t>(height)};

        color_rotator += 0.001;

        VkRenderPassBeginInfo render_pass_begin_info = LunaLuxVulkanLib::vkClearColour(render_area,std::sin( color_rotator + CIRCLE_THIRD_1 ) * 0.5 + 0.5,
                                                                                      std::sin( color_rotator + CIRCLE_THIRD_2 ) * 0.5 + 0.5,
                                                                                      std::sin( color_rotator + CIRCLE_THIRD_3 ) * 0.5 + 0.5,1.0f);

        vkCmdBeginRenderPass( command_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE );

        vkCmdEndRenderPass( command_buffer );

        vkEndCommandBuffer( command_buffer );


        VkSubmitInfo submit_info {};
        submit_info.sType					= VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.waitSemaphoreCount		= 0;
        submit_info.pWaitSemaphores			= nullptr;
        submit_info.pWaitDstStageMask		= nullptr;
        submit_info.commandBufferCount		= 1;
        submit_info.pCommandBuffers			= &command_buffer;
        submit_info.signalSemaphoreCount	= 1;
        submit_info.pSignalSemaphores		= &render_complete_semaphore;

        LunaLuxVulkanLib::frameSubmit({ render_complete_semaphore },submit_info);
    }

    vkDestroySemaphore( LunaLuxVulkanLib::getDevice(), render_complete_semaphore, nullptr );
    vkDestroyCommandPool( LunaLuxVulkanLib::getDevice(), command_pool, nullptr );

    LunaLuxVulkanLib::destroyContext();

    return 0;
}