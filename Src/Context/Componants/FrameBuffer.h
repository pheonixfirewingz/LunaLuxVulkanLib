//
// Created by luket on 22/01/2021.
//

#ifndef GITIGNORE_FRAMEBUFFER_H
#define GITIGNORE_FRAMEBUFFER_H
//TODO: write documentation
#include <vulkan/vulkan_core.h>
#include <vector>
#include "Device.h"
#include "SwapChain.h"
#include "DepthHandler.h"
namespace LunaLuxVulkanLib
{
    class FrameBuffer
    {
    private:
        Device* device = nullptr;
        std::vector<VkFramebuffer> frameBuffers;
        void create(SwapChain* swapChain,DepthHandler* depthHandler,VkRenderPass renderPass, uint32_t, uint32_t);
        void Destroy();
    public:
        FrameBuffer(Device * device,SwapChain* swapChain,VkRenderPass renderPass,DepthHandler* depthHandler, uint32_t, uint32_t);
        void reset(SwapChain* swapChain,VkRenderPass renderPass,DepthHandler* depthHandler, uint32_t width, uint32_t height);
        ~FrameBuffer();

        const std::vector<VkFramebuffer> &getFrameBuffers() const;
    };
};
#endif //GITIGNORE_FRAMEBUFFER_H
