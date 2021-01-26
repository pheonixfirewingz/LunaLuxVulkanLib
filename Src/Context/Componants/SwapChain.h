//
// Created by luket on 22/01/2021.
//

#ifndef GITIGNORE_SWAPCHAIN_H
#define GITIGNORE_SWAPCHAIN_H
#include <vulkan/vulkan_core.h>
#include <vector>
#include "Surface.h"
#include "Device.h"
//TODO: write documentation
namespace LunaLuxVulkanLib
{
    class SwapChain
    {
    private:
        Device* device = nullptr;
        VkSwapchainKHR swapchain = nullptr;
        VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR;
        uint32_t ImageCount = 0;
        std::vector<VkImage>				swapChain_images;
        std::vector<VkImageView>			swapChain_image_views;
        void create(Surface* surface,uint32_t width,uint32_t height);
        void Destroy();
    public:
        SwapChain(Surface* surface,Device* device,uint32_t width,uint32_t height);
        void reset(Surface* surface,uint32_t width,uint32_t height);
        ~SwapChain();

        VkSwapchainKHR getSwapchain() const;

        uint32_t getImageCount() const;

        const std::vector<VkImageView> &getSwapChainImageViews() const;
    };
};
#endif //GITIGNORE_SWAPCHAIN_H
