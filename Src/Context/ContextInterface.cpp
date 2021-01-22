//
// Created by luket on 09/01/2021.
//

//TODO: write documentation
#include "ContextInterface.h"
#include "Interface/Platform.h"
#include "../Common/Utils.h"
#include "Debug.h"

#include <stdexcept>

namespace LunaLuxVulkanLib
{

    void ContextInterface::createContext(bool debug, LunaLuxWindowLib::Window *window)
    {
        instance = new Instance(debug);
        auto[width, height] = window->GetWindowSize();
        device = new Device(debug,instance->get());
        if (debug)createDebug(instance->get());
        vkGetDeviceQueue(device->getDev(), device->getGraphicFamilyIndex(), 0, &graphicQueue);
        surface = new Surface(instance->get(), device, window);
        swapChain = new SwapChain(surface,device,width,height);
        debug_wrapper(debug, "vkGetSwapchainImagesKHR",
                      vkGetSwapchainImagesKHR(device->getDev(), swapChain->getSwapchain(), &ImageCount, nullptr))
        swapChain_images.resize(ImageCount);
        swapChain_image_views.resize(ImageCount);
        debug_wrapper(debug, "vkGetSwapchainImagesKHR",
                      vkGetSwapchainImagesKHR(device->getDev(), swapChain->getSwapchain(), &ImageCount, swapChain_images.data()))

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

            debug_wrapper(debug, "vkCreateImageView",
                          vkCreateImageView(device->getDev(), &image_view_create_info, nullptr,
                                            &swapChain_image_views[i]))
        }
        bool stencilAvailable = false;
        {
            std::vector<VkFormat> try_formats
                    {
                            VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT, VK_FORMAT_D16_UNORM_S8_UINT, VK_FORMAT_D32_SFLOAT, VK_FORMAT_D16_UNORM
                    };
            for (auto f: try_formats)
            {
                VkFormatProperties format_properties{};
                vkGetPhysicalDeviceFormatProperties(device->getPDev(), f, &format_properties
                );
                if (format_properties.
                        optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
                        )
                {
                    depthFormat = f;
                    break;
                }
            }
            if (depthFormat == VK_FORMAT_UNDEFINED)
            { throw std::runtime_error("Depth stencil format not selected."); }

            if ((depthFormat == VK_FORMAT_D32_SFLOAT_S8_UINT) || (depthFormat == VK_FORMAT_D24_UNORM_S8_UINT) ||
                (depthFormat == VK_FORMAT_D16_UNORM_S8_UINT) || (depthFormat == VK_FORMAT_S8_UINT))
            {
                stencilAvailable = true;
            }
        }

        VkImageCreateInfo image_create_info{};
        image_create_info.
                sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        image_create_info.
                flags = 0;
        image_create_info.
                imageType = VK_IMAGE_TYPE_2D;
        image_create_info.
                format = depthFormat;
        image_create_info.extent.
                width = width;
        image_create_info.extent.
                height = height;
        image_create_info.extent.
                depth = 1;
        image_create_info.
                mipLevels = 1;
        image_create_info.
                arrayLayers = 1;
        image_create_info.
                samples = VK_SAMPLE_COUNT_1_BIT;
        image_create_info.
                tiling = VK_IMAGE_TILING_OPTIMAL;
        image_create_info.
                usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        image_create_info.
                sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        image_create_info.
                queueFamilyIndexCount = VK_QUEUE_FAMILY_IGNORED;
        image_create_info.
                pQueueFamilyIndices = nullptr;
        image_create_info.
                initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

        debug_wrapper(debug, "vkCreateImage", vkCreateImage(device->getDev(), &image_create_info, nullptr, &depthImage))

        VkMemoryRequirements image_memory_requirements{};
        vkGetImageMemoryRequirements(device->getDev(), depthImage, &image_memory_requirements);

        uint32_t memory_index = device->findGpuMemoryType(image_memory_requirements.memoryTypeBits,
                                                          VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        VkMemoryAllocateInfo memory_allocate_info{};
        memory_allocate_info.
                sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memory_allocate_info.
                allocationSize = image_memory_requirements.size;
        memory_allocate_info.
                memoryTypeIndex = memory_index;

        vkAllocateMemory(device->getDev(), &memory_allocate_info, nullptr, &depthImageDeviceMemory);
        vkBindImageMemory(device->getDev(), depthImage, depthImageDeviceMemory, 0);

        VkImageViewCreateInfo image_view_create_info{};
        image_view_create_info.
                sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        image_view_create_info.
                image = depthImage;
        image_view_create_info.
                viewType = VK_IMAGE_VIEW_TYPE_2D;
        image_view_create_info.
                format = depthFormat;
        image_view_create_info.components.
                r = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.
                g = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.
                b = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.
                a = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.subresourceRange.
                aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | (stencilAvailable ? VK_IMAGE_ASPECT_STENCIL_BIT : 0);
        image_view_create_info.subresourceRange.
                baseMipLevel = 0;
        image_view_create_info.subresourceRange.
                levelCount = 1;
        image_view_create_info.subresourceRange.
                baseArrayLayer = 0;
        image_view_create_info.subresourceRange.
                layerCount = 1;

        vkCreateImageView(device->getDev(), &image_view_create_info, nullptr, &depthImageView);
    }

    void ContextInterface::destroyContext(bool debug)
    {
        vkDestroyImageView( device->getDev(), depthImageView, nullptr );
        vkFreeMemory( device->getDev(), depthImageDeviceMemory, nullptr );
        vkDestroyImage( device->getDev(), depthImage, nullptr );
        for (auto view : swapChain_image_views) vkDestroyImageView(device->getDev(), view, nullptr);
        delete swapChain;
        delete surface;
        delete device;
        if (debug) destroyDebug(instance->get());
        delete instance;
    }

    Instance *ContextInterface::getInstance() const
    {
        return instance;
    }

    Device *ContextInterface::getDevice() const
    {
        return device;
    }

    Surface *ContextInterface::getSurface() const
    {
        return surface;
    }

    VkFormat ContextInterface::getDepthFormat() const
    {
        return depthFormat;
    }

    const VkQueue ContextInterface::getGraphicQueue() const
    {
        return graphicQueue;
    }

    const VkImage ContextInterface::getDepthImage() const
    {
        return depthImage;
    }

    const VkDeviceMemory ContextInterface::getDepthImageDeviceMemory() const
    {
        return depthImageDeviceMemory;
    }

    const VkImageView ContextInterface::getDepthImageView() const
    {
        return depthImageView;
    }

    const std::vector<VkImage> &ContextInterface::getSwapChainImages() const
    {
        return swapChain_images;
    }

    const std::vector<VkImageView> &ContextInterface::getSwapChainImageViews() const
    {
        return swapChain_image_views;
    }

    SwapChain *ContextInterface::getSwapChain() const
    {
        return swapChain;
    }

    uint32_t ContextInterface::getImageCount() const
    {
        return ImageCount;
    }
}