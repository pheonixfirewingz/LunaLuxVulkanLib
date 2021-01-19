//
// Created by luket on 09/01/2021.
//

//TODO: write documentation
#include <array>
#include "Context/ContextInterface.h"
#include "Common/Utils.h"
#include "VulkanLib.h"
#include "VulkanLibRenderComands.h"

namespace LunaLuxVulkanLib
{
    bool IDebug{false};
    VkFence	swapChainFence		= VK_NULL_HANDLE;
    uint32_t currentFrame = 0;
    ContextInterface* context;

    void createContext(bool debug, LunaLuxWindowLib::Window * window)
    {
        IDebug = debug;
        context = new ContextInterface();
        context->createContext(debug,window);
    }

    void updateContext(LunaLuxWindowLib::Window * window)
    {
        context->updateContext(IDebug,window);
    }

    const uint32_t getFamilyIndex()
    {
        return context->getFamilyIndex();
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
        context->destroyContext(IDebug);
    }

    void frameBegin()
    {
        debug_wrapper(IDebug, vkAcquireNextImageKHR(context->getDevice(),context->getSwapchain(),UINT64_MAX,VK_NULL_HANDLE,
                                                    swapChainFence,&currentFrame ) )
        debug_wrapper(IDebug, vkWaitForFences( context->getDevice(), 1, &swapChainFence, VK_TRUE, UINT64_MAX ) )
        debug_wrapper(IDebug, vkResetFences(  context->getDevice(), 1, &swapChainFence ) )
        debug_wrapper(IDebug, vkQueueWaitIdle(context->getGraphicQueue()))
    }

    void frameSubmit(std::vector<VkSemaphore> wait_semaphores,VkSubmitInfo submitInfo)
    {
        vkQueueSubmit( context->getGraphicQueue(), 1, &submitInfo, VK_NULL_HANDLE );
    }

    VkRenderPassBeginInfo vkClearColour(VkRect2D renderArea,float r, float g, float b, float a)
    {
        std::array<VkClearValue, 2> clear_values {};
        clear_values[ 0 ].depthStencil.depth		= 0.0f;
        clear_values[ 0 ].depthStencil.stencil		= 0;
        clear_values[ 1 ].color.float32[ 0 ]		= r;
        clear_values[ 1 ].color.float32[ 1 ]		= g;
        clear_values[ 1 ].color.float32[ 2 ]		= b;
        clear_values[ 1 ].color.float32[ 3 ]		= a;

        VkRenderPassBeginInfo render_pass_begin_info {};
        render_pass_begin_info.sType				= VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        render_pass_begin_info.renderPass			= w->GetVulkanRenderPass();
        render_pass_begin_info.framebuffer			= w->GetVulkanActiveFramebuffer();
        render_pass_begin_info.renderArea			= renderArea;
        render_pass_begin_info.clearValueCount		= clear_values.size();
        render_pass_begin_info.pClearValues			= clear_values.data();

        return render_pass_begin_info;
    }
}
