//
// Created by luket on 22/01/2021.
//

//TODO: write documentation
#include <stdexcept>
#include "DepthHandler.h"


namespace LunaLuxVulkanLib
{
    DepthHandler::DepthHandler(Device* device,uint32_t width,uint32_t height): device(device)
    {
        {
            VkFormat try_formats[] =
            {
                    VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT, VK_FORMAT_D16_UNORM_S8_UINT,
                    VK_FORMAT_D32_SFLOAT, VK_FORMAT_D16_UNORM
            };
            for (auto f: try_formats)
            {
                VkFormatProperties format_properties{};
                vkGetPhysicalDeviceFormatProperties(device->getPDev(), f, &format_properties);
                if (format_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
                {
                    depthFormat = f;
                    break;
                }
            }
            if (depthFormat == VK_FORMAT_UNDEFINED)
                throw std::runtime_error("Depth stencil format not selected.");

            if ((depthFormat == VK_FORMAT_D32_SFLOAT_S8_UINT) || (depthFormat == VK_FORMAT_D24_UNORM_S8_UINT) ||
                (depthFormat == VK_FORMAT_D16_UNORM_S8_UINT) || (depthFormat == VK_FORMAT_S8_UINT))
                stencilAvailable = true;
        }
        create(width,height);
    }

    void DepthHandler::reset(uint32_t width,uint32_t height)
    {
        Destroy();
        create(width,height);
    }

    DepthHandler::~DepthHandler()
    {
        Destroy();
    }

    VkFormat DepthHandler::getDepthFormat() const
    {
        return depthFormat;
    }


    const VkImageView DepthHandler::getDepthImageView() const
    {
        return depthImageView;
    }

    bool DepthHandler::isStencilAvailable() const
    {
        return stencilAvailable;
    }

    void DepthHandler::create(uint32_t width,uint32_t height)
    {
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

        vkCreateImage(device->getDev(), &image_create_info, nullptr, &depthImage);

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

    void DepthHandler::Destroy()
    {
        vkDestroyImageView( device->getDev(), depthImageView, nullptr );
        vkFreeMemory( device->getDev(), depthImageDeviceMemory, nullptr );
        vkDestroyImage( device->getDev(), depthImage, nullptr );
    }
};
