//
// Created by luket on 22/01/2021.
//
//TODO: write documentation
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

        vkGetSwapchainImagesKHR(device->getDev(), swapchain, &ImageCount, nullptr);
        swapChain_images.resize(ImageCount);
        swapChain_image_views.resize(ImageCount);
        vkGetSwapchainImagesKHR(device->getDev(), swapchain, &ImageCount,swapChain_images.data());

        for (uint32_t i = 0; i < ImageCount; ++i)
        {
            VkImageViewCreateInfo image_view_create_info{};
            image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            image_view_create_info.image = swapChain_images[i];
            image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
            image_view_create_info.format = surface->getSurfaceFormat().format;
            image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            image_view_create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            image_view_create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            image_view_create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            image_view_create_info.subresourceRange.baseMipLevel = 0;
            image_view_create_info.subresourceRange.levelCount = 1;
            image_view_create_info.subresourceRange.baseArrayLayer = 0;
            image_view_create_info.subresourceRange.layerCount = 1;

            vkCreateImageView(device->getDev(), &image_view_create_info, nullptr,&swapChain_image_views[i]);
        }
    }

    void SwapChain::Destroy()
    {
        for (auto view : swapChain_image_views) vkDestroyImageView(device->getDev(), view, nullptr);
        vkDestroySwapchainKHR(device->getDev(), swapchain, nullptr);
    }

    SwapChain::SwapChain(Surface* surface,Device* device,uint32_t width,uint32_t height) : device(device)
    {
        {
            uint32_t present_mode_count = 0;
            VkPresentModeKHR* present_mode_list;
            vkGetPhysicalDeviceSurfacePresentModesKHR(device->getPDev(), surface->getSurface(),&present_mode_count, nullptr);
            present_mode_list = new VkPresentModeKHR[present_mode_count];
            vkGetPhysicalDeviceSurfacePresentModesKHR(device->getPDev(), surface->getSurface(),&present_mode_count,present_mode_list);
            for (uint32_t i = 0; i <=present_mode_count; i++)
                if (present_mode_list[i] == VK_PRESENT_MODE_MAILBOX_KHR)
                    present_mode = present_mode_list[i];
        }
        create(surface,width,height);
    }

    void SwapChain::reset(Surface* surface,uint32_t width, uint32_t height)
    {
        Destroy();
        create(surface,width,height);
    }

    SwapChain::~SwapChain()
    {
        Destroy();
    }

    VkSwapchainKHR SwapChain::getSwapchain() const
    {
        return swapchain;
    }

    uint32_t SwapChain::getImageCount() const
    {
        return ImageCount;
    }

    const std::vector<VkImageView> &SwapChain::getSwapChainImageViews() const
    {
        return swapChain_image_views;
    }
};
