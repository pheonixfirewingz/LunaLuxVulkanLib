//
// Created by luket on 22/01/2021.
//

//TODO: write documentation
#include <array>
#include "FrameBuffer.h"

namespace LunaLuxVulkanLib
{

    FrameBuffer::FrameBuffer(Device *device, SwapChain *swapChain,VkRenderPass renderPass,
                             DepthHandler* depthHandler, uint32_t width, uint32_t height) :device(device)
    {
        create(swapChain,depthHandler,renderPass,width,height);
    }

    FrameBuffer::~FrameBuffer()
    {
        Destroy();
    }

    void FrameBuffer::create(SwapChain* swapChain,DepthHandler* depthHandler,
                             VkRenderPass renderPass, uint32_t width, uint32_t height)
    {
        uint32_t ImageCount = swapChain->getImageCount();
        frameBuffers.resize( ImageCount );
        for( uint32_t i=0; i < ImageCount; ++i )
        {
            std::array<VkImageView, 2> attachments {};
            attachments[ 0 ]	= depthHandler->getDepthImageView();
            attachments[ 1 ]	= swapChain->getSwapChainImageViews()[ i ];

            VkFramebufferCreateInfo framebuffer_create_info {};
            framebuffer_create_info.sType			= VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebuffer_create_info.renderPass		= renderPass;
            framebuffer_create_info.attachmentCount	= attachments.size();
            framebuffer_create_info.pAttachments	= attachments.data();
            framebuffer_create_info.width			= width;
            framebuffer_create_info.height			= height;
            framebuffer_create_info.layers			= 1;

            vkCreateFramebuffer( device->getDev(), &framebuffer_create_info, nullptr, &frameBuffers[ i ] );
        }
    }


    void FrameBuffer::Destroy()
    {
        for( auto f : frameBuffers ) vkDestroyFramebuffer( device->getDev(), f, nullptr );
    }

    void FrameBuffer::reset(SwapChain* swapChain,VkRenderPass renderPass,DepthHandler* depthHandler, uint32_t width, uint32_t height)
    {
        Destroy();
        for(int i = 0; i <= frameBuffers.size(); i++) frameBuffers.pop_back();
        create(swapChain,depthHandler,renderPass,width,height);
    }

    const std::vector<VkFramebuffer> &FrameBuffer::getFrameBuffers() const
    {
        return frameBuffers;
    }
}
