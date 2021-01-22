//
// Created by luket on 22/01/2021.
//

//TODO: write documentation
#include <vector>
#include "SwapChain.h"


namespace LunaLuxVulkanLib
{

    void SwapChain::create(Surface* surface,uint32_t width,uint32_t height)
    {
        VkSwapchainCreateInfoKHR swapchain_create_info{};
        swapchain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchain_create_info.surface = surface->getSurface();
        swapchain_create_info.minImageCount = surface->getSurfaceCapabilities().minImageCount;
        swapchain_create_info.imageFormat = surface->getSurfaceFormat().format;
        swapchain_create_info.imageColorSpace = surface->getSurfaceFormat().colorSpace;
        swapchain_create_info.imageExtent.width = width;
        swapchain_create_info.imageExtent.height = height;
        swapchain_create_info.imageArrayLayers = 1;
        swapchain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapchain_create_info.queueFamilyIndexCount = 0;
        swapchain_create_info.pQueueFamilyIndices = nullptr;
        swapchain_create_info.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        swapchain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapchain_create_info.presentMode = present_mode;
        swapchain_create_info.clipped = VK_TRUE;
        swapchain_create_info.oldSwapchain = VK_NULL_HANDLE;

        vkCreateSwapchainKHR(device->getDev(), &swapchain_create_info, nullptr, &swapchain);
    }

    void SwapChain::Destroy()
    {
        vkDestroySwapchainKHR(device->getDev(), swapchain, nullptr);
    }

    SwapChain::SwapChain(Surface* surface,Device* device,uint32_t width,uint32_t height) : device(device)
    {
        {
            uint32_t present_mode_count = 0;
            vkGetPhysicalDeviceSurfacePresentModesKHR(device->getPDev(), surface->getSurface(),&present_mode_count, nullptr);
            std::vector<VkPresentModeKHR> present_mode_list(present_mode_count);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device->getPDev(), surface->getSurface(),&present_mode_count,present_mode_list.data());
            for (auto m : present_mode_list) if (m == VK_PRESENT_MODE_MAILBOX_KHR) present_mode = m;
        }
        create(surface,width,height);
    }

    void SwapChain::reset(Surface* surface,uint32_t width, uint32_t height)
    {
        printf("swapChain resize needed\n");
        //Destroy();
        //create(surface,width,height);
    }

    SwapChain::~SwapChain()
    {
        Destroy();
    }

    const VkSwapchainKHR SwapChain::getSwapchain() const
    {
        return swapchain;
    }

    VkPresentModeKHR SwapChain::getPresentMode() const
    {
        return present_mode;
    }
};
