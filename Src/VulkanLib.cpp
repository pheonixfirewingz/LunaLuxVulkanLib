//
// Created by luket on 09/01/2021.
//

//TODO: write documentation
#include <array>
#include <stdexcept>
#include "Context/ContextInterface.h"
#include "Common/Utils.h"
#include "VulkanLib.h"
#include "VulkanLibRenderComands.h"

namespace LunaLuxVulkanLib
{
    bool IDebug{false};
    VkRect2D render_area {};
    VkFence	swapChainFence = nullptr;
    uint32_t currentFrame = 0;
    ContextInterface* context;
    VkRenderPass renderPass = nullptr;
    std::vector<VkFramebuffer> frameBuffers;
    VkSwapchainKHR swapchain;

    void createContext(bool debug, LunaLuxWindowLib::Window * window)
    {
        IDebug = debug;
        context = new ContextInterface();

        auto[width,height] = window->GetWindowSize();
        render_area.offset.x		= 0;
        render_area.offset.y		= 0;
        render_area.extent			= {static_cast<uint32_t>(width),static_cast<uint32_t>(height)};

        context->createContext(debug,window);

        std::array<VkAttachmentDescription, 2> attachments {};
        attachments[ 0 ].flags						= 0;
        attachments[ 0 ].format						= context->getDepthFormat();
        attachments[ 0 ].samples					= VK_SAMPLE_COUNT_1_BIT;
        attachments[ 0 ].loadOp						= VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachments[ 0 ].storeOp					= VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachments[ 0 ].stencilLoadOp				= VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachments[ 0 ].stencilStoreOp				= VK_ATTACHMENT_STORE_OP_STORE;
        attachments[ 0 ].initialLayout				= VK_IMAGE_LAYOUT_UNDEFINED;
        attachments[ 0 ].finalLayout				= VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        attachments[ 1 ].flags						= 0;
        attachments[ 1 ].format						= context->getSurfaceFormat().format;
        attachments[ 1 ].samples					= VK_SAMPLE_COUNT_1_BIT;
        attachments[ 1 ].loadOp						= VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachments[ 1 ].storeOp					= VK_ATTACHMENT_STORE_OP_STORE;
        attachments[ 1 ].initialLayout				= VK_IMAGE_LAYOUT_UNDEFINED;
        attachments[ 1 ].finalLayout				= VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;


        VkAttachmentReference sub_pass_0_depth_stencil_attachment {};
        sub_pass_0_depth_stencil_attachment.attachment	= 0;
        sub_pass_0_depth_stencil_attachment.layout		= VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        std::array<VkAttachmentReference, 1> sub_pass_0_color_attachments {};
        sub_pass_0_color_attachments[ 0 ].attachment	= 1;
        sub_pass_0_color_attachments[ 0 ].layout		= VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        std::array<VkSubpassDescription, 1> sub_passes {};
        sub_passes[ 0 ].pipelineBindPoint			= VK_PIPELINE_BIND_POINT_GRAPHICS;
        sub_passes[ 0 ].colorAttachmentCount		= sub_pass_0_color_attachments.size();
        sub_passes[ 0 ].pColorAttachments			= sub_pass_0_color_attachments.data();
        sub_passes[ 0 ].pDepthStencilAttachment		= &sub_pass_0_depth_stencil_attachment;


        VkRenderPassCreateInfo render_pass_create_info {};
        render_pass_create_info.sType				= VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        render_pass_create_info.attachmentCount		= attachments.size();
        render_pass_create_info.pAttachments		= attachments.data();
        render_pass_create_info.subpassCount		= sub_passes.size();
        render_pass_create_info.pSubpasses			= sub_passes.data();

        debug_wrapper(debug, vkCreateRenderPass( context->getDevice(), &render_pass_create_info, nullptr, &renderPass ) )

        uint32_t ImageCount = context->getImageCount();
        frameBuffers.resize( ImageCount );
        for( uint32_t i=0; i < ImageCount; ++i )
        {
            std::array<VkImageView, 2> attachments {};
            attachments[ 0 ]	= context->getDepthImageView();
            attachments[ 1 ]	= context->getSwapChainImageViews()[ i ];

            VkFramebufferCreateInfo framebuffer_create_info {};
            framebuffer_create_info.sType			= VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebuffer_create_info.renderPass		= renderPass;
            framebuffer_create_info.attachmentCount	= attachments.size();
            framebuffer_create_info.pAttachments	= attachments.data();
            framebuffer_create_info.width			= width;
            framebuffer_create_info.height			= height;
            framebuffer_create_info.layers			= 1;

            debug_wrapper(debug, vkCreateFramebuffer( context->getDevice(), &framebuffer_create_info, nullptr, &frameBuffers[ i ] ) )
        }
        swapchain = context->getSwapchain();
    }

    void updateContext(LunaLuxWindowLib::Window * window)
    {
        auto[width,height] = window->GetWindowSize();
        render_area.extent = {static_cast<uint32_t>(width),static_cast<uint32_t>(height)};
        context->updateContext(IDebug,window);
    }

    const VkInstance LunaLuxVulkanLib::getInstance()
    {
        return context->getInstance();
    }

    const VkDevice getDevice()
    {
        return context->getDevice();
    }

    void destroyContext()
    {
        vkQueueWaitIdle(context->getGraphicQueue());
        for( auto f : frameBuffers ) vkDestroyFramebuffer( context->getDevice(), f, nullptr );
        vkDestroyRenderPass(context->getDevice(), renderPass, nullptr );
        vkDestroyFence(context->getDevice(),swapChainFence, nullptr);
        context->destroyContext(IDebug);
    }

    void frameBegin(VkFence fence)
    {
        debug_wrapper(IDebug, vkAcquireNextImageKHR(context->getDevice(),context->getSwapchain(),UINT64_MAX,VK_NULL_HANDLE,
                                                    fence,&currentFrame ) )
        debug_wrapper(IDebug, vkWaitForFences( context->getDevice(), 1, &fence, VK_TRUE, UINT64_MAX ) )
        debug_wrapper(IDebug, vkResetFences(  context->getDevice(), 1, &fence ) )
        debug_wrapper(IDebug, vkQueueWaitIdle(context->getGraphicQueue()))
    }

    void frameSubmit(std::vector<VkSemaphore> wait_semaphores,VkSubmitInfo submitInfo)
    {
        vkQueueSubmit( context->getGraphicQueue(), 1, &submitInfo, VK_NULL_HANDLE );
        VkResult present_result = VkResult::VK_RESULT_MAX_ENUM;


        VkPresentInfoKHR present_info {};
        present_info.sType					= VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        present_info.waitSemaphoreCount		= wait_semaphores.size();
        present_info.pWaitSemaphores		= wait_semaphores.data();
        present_info.swapchainCount			= 1;
        present_info.pSwapchains			= &swapchain;
        present_info.pImageIndices			= &currentFrame;
        present_info.pResults				= &present_result;

        debug_wrapper(IDebug, vkQueuePresentKHR( context->getGraphicQueue(), &present_info ) )
        debug_wrapper(IDebug, present_result );
    }

    VkCommandPool vkGenCommandPool(VkCommandPoolCreateFlags flags)
    {
        VkCommandPool command_pool;

        VkCommandPoolCreateInfo pool_create_info {};
        pool_create_info.sType				= VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        pool_create_info.flags				= flags;
        pool_create_info.queueFamilyIndex	= context->getFamilyIndex();

        vkCreateCommandPool(context->getDevice(), &pool_create_info, nullptr, &command_pool );
        return command_pool;
    }

    VkRenderPassBeginInfo vkClearColour(float r, float g, float b, float a)
    {
        std::vector<VkClearValue> clear_values(2);
        clear_values[ 0 ].depthStencil.depth		= 0.0f;
        clear_values[ 0 ].depthStencil.stencil		= 0;
        clear_values[ 1 ].color.float32[ 0 ]		= r;
        clear_values[ 1 ].color.float32[ 1 ]		= g;
        clear_values[ 1 ].color.float32[ 2 ]		= b;
        clear_values[ 1 ].color.float32[ 3 ]		= a;

        VkRenderPassBeginInfo render_pass_begin_info {};
        render_pass_begin_info.sType				= VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        render_pass_begin_info.renderPass			= renderPass;
        render_pass_begin_info.framebuffer			= frameBuffers[currentFrame];
        render_pass_begin_info.renderArea			= render_area;
        render_pass_begin_info.clearValueCount		= clear_values.size();
        render_pass_begin_info.pClearValues         = clear_values.data();

        return render_pass_begin_info;
    }

    void vkQueueWaitIdle()
    {
        debug_wrapper(IDebug, vkQueueWaitIdle(context->getGraphicQueue()))
    }
}
