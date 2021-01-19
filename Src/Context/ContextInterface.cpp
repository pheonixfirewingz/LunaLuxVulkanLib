//
// Created by luket on 09/01/2021.
//

//TODO: write documentation
#include "ContextInterface.h"
#include "../Common/Utils.h"
#include "GPU.h"
#include "Debug.h"

#include <stdexcept>
namespace LunaLuxVulkanLib
{

    void ContextInterface::createContext(bool debug, LunaLuxWindowLib::Window * window)
    {
        {
            std::vector<const char *> instextend;

            instextend.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
            if(debug) instextend.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
#ifdef LLE_WINDOWS
            instextend.push_back("VK_KHR_win32_surface");
#endif
            uint32_t l_count = 0;
            if (debug) vkEnumerateInstanceLayerProperties(&l_count, nullptr);
            std::vector<const char *> instlayname(l_count);
            std::vector<VkLayerProperties> instlay(l_count);
            if (debug)
            {
                vkEnumerateInstanceLayerProperties(&l_count, instlay.data());
                int temp = 0;
                for (auto &i: instlay)
                {
                    instlayname[temp] = i.layerName;
                    temp++;
                }
                {
                    auto temp_ = instlayname.back();
                    instlayname[0] = temp_;
                    instlayname.pop_back();
                }
                {
                    auto temp_ = instlayname.back();
                    instlayname[1] = temp_;
                    instlayname.pop_back();
                }
            }

            // initialize the VkApplicationInfo structure
            VkApplicationInfo app_info = {};
            app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            app_info.pNext = nullptr;
            app_info.pApplicationName = "LunaLuxVulkanLib_app";
            app_info.applicationVersion = 1;
            app_info.pEngineName = "LunaLuxVulkanLib";
            app_info.engineVersion = 1;
            app_info.apiVersion = VK_API_VERSION_1_2;
            // initialize the VkInstanceCreateInfo structure
            VkInstanceCreateInfo inst_info = {};
            inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            inst_info.pNext = nullptr;
            inst_info.flags = 0;
            inst_info.pApplicationInfo = &app_info;
            inst_info.enabledExtensionCount = instextend.size();
            inst_info.ppEnabledExtensionNames = instextend.data();
            if (debug)
            {
                inst_info.enabledLayerCount = instlayname.size();
                inst_info.ppEnabledLayerNames = instlayname.data();
            }
            else
            {
                inst_info.enabledLayerCount = 0;
                inst_info.ppEnabledLayerNames = nullptr;
            }
            debug_wrapper(debug, vkCreateInstance(&inst_info, nullptr, &instance))
        }
        if(debug) createDebug(instance);

        {
            std::vector<const char *> devextend;
            devextend.push_back( VK_KHR_SWAPCHAIN_EXTENSION_NAME );
            {
                p_device = getPDevice(instance);
                vkGetPhysicalDeviceProperties(p_device, &properties);
                vkGetPhysicalDeviceFeatures(p_device,&deviceFeatures);
                float queue[]{1.0f};
                VkDeviceQueueCreateInfo queue_info = {};
                queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                queue_info.pNext = nullptr;
                queue_info.flags = 0;
                {
                    std::vector<VkQueueFamilyProperties> list = get_vector_list(p_device);

                    for (int i = 0; i < list.size(); i++)
                        if (list[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                        {
                            queue_info.queueFamilyIndex = i;
                            family_index = i;
                            break;
                        }
                }
                queue_info.pQueuePriorities = queue;
                queue_info.queueCount = 1;

                uint32_t l_count = 0;
                if (debug) vkEnumerateDeviceLayerProperties(p_device,&l_count, nullptr);
                std::vector<const char *> devlayname(l_count);
                std::vector<VkLayerProperties> devlay(l_count);
                if (debug)
                {
                    vkEnumerateDeviceLayerProperties(p_device,&l_count, devlay.data());
                    int temp = 0;
                    for (auto &i: devlay)
                    {
                        devlayname[temp] = i.layerName;
                        temp++;
                    }
                }

                VkDeviceCreateInfo deviceCreateInfo = {};
                deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
                deviceCreateInfo.pNext = nullptr;
                deviceCreateInfo.flags = 0;
                deviceCreateInfo.enabledExtensionCount = devextend.size();
                deviceCreateInfo.enabledLayerCount = devlayname.size();
                deviceCreateInfo.ppEnabledExtensionNames = devextend.data();
                deviceCreateInfo.ppEnabledLayerNames = devlayname.data();
                deviceCreateInfo.pEnabledFeatures = nullptr;
                deviceCreateInfo.pQueueCreateInfos = &queue_info;
                deviceCreateInfo.queueCreateInfoCount = 1;

                debug_wrapper(debug, vkCreateDevice(p_device, &deviceCreateInfo, nullptr, &device))
            }
            vkGetDeviceQueue( device, family_index, 0, &graphicQueue );

#ifdef LLE_WINDOWS
            createWin32Surf(debug,instance,&surface,window);
#endif
            VkBool32 support{false};
            vkGetPhysicalDeviceSurfaceSupportKHR(p_device,family_index,surface,&support);
            if(!support) throw std::runtime_error("something is not right with surface creation");
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(p_device,surface,&surfaceCapabilities);
            llegetSurfaceFormat(p_device,surface,&surfaceFormat);
        }
        {
            VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR;
            {
                uint32_t present_mode_count = 0;
                debug_wrapper(debug, vkGetPhysicalDeviceSurfacePresentModesKHR( p_device, surface, &present_mode_count, nullptr ) )
                std::vector<VkPresentModeKHR> present_mode_list( present_mode_count );
                debug_wrapper(debug, vkGetPhysicalDeviceSurfacePresentModesKHR( p_device ,surface, &present_mode_count, present_mode_list.data() ) )
                for( auto m : present_mode_list ) if( m == VK_PRESENT_MODE_MAILBOX_KHR ) present_mode = m;
            }
            auto[width,height] = window->GetWindowSize();
            VkSwapchainCreateInfoKHR swapchain_create_info{};
            swapchain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            swapchain_create_info.surface = surface;
            swapchain_create_info.minImageCount = surfaceCapabilities.minImageCount;
            swapchain_create_info.imageFormat = surfaceFormat.format;
            swapchain_create_info.imageColorSpace = surfaceFormat.colorSpace;
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

            debug_wrapper(debug, vkCreateSwapchainKHR(device, &swapchain_create_info, nullptr, &swapchain))

            uint32_t ImageCount;
            debug_wrapper(debug, vkGetSwapchainImagesKHR(device, swapchain, &ImageCount, nullptr))
            swapChain_images.resize(ImageCount);
            swapChain_image_views.resize(ImageCount);
            debug_wrapper(debug, vkGetSwapchainImagesKHR(device, swapchain, &ImageCount, swapChain_images.data()))

            for( uint32_t i=0; i < ImageCount; ++i )
            {
                VkImageViewCreateInfo image_view_create_info {};
                image_view_create_info.sType				= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
                image_view_create_info.image				= swapChain_images[ i ];
                image_view_create_info.viewType				= VK_IMAGE_VIEW_TYPE_2D;
                image_view_create_info.format				= surfaceFormat.format;
                image_view_create_info.components.r			= VK_COMPONENT_SWIZZLE_IDENTITY;
                image_view_create_info.components.g			= VK_COMPONENT_SWIZZLE_IDENTITY;
                image_view_create_info.components.b			= VK_COMPONENT_SWIZZLE_IDENTITY;
                image_view_create_info.components.a			= VK_COMPONENT_SWIZZLE_IDENTITY;
                image_view_create_info.subresourceRange.aspectMask			= VK_IMAGE_ASPECT_COLOR_BIT;
                image_view_create_info.subresourceRange.baseMipLevel		= 0;
                image_view_create_info.subresourceRange.levelCount			= 1;
                image_view_create_info.subresourceRange.baseArrayLayer		= 0;
                image_view_create_info.subresourceRange.layerCount			= 1;

                debug_wrapper(debug, vkCreateImageView( device, &image_view_create_info, nullptr, &swapChain_image_views[ i ] ) )
            }
        }
        {
            //TODO: create depth stencil image
        }
    }

    void ContextInterface::updateContext([[maybe_unused]]bool debug, [[maybe_unused]]LunaLuxWindowLib::Window * window)
    {

    }

    void ContextInterface::destroyContext(bool debug)
    {
        for( auto view : swapChain_image_views ) vkDestroyImageView( device, view, nullptr );
        vkDestroySwapchainKHR(device,swapchain, nullptr);
        vkDestroyDevice(device, nullptr);
        vkDestroySurfaceKHR(instance,surface, nullptr);
        if(debug) destroyDebug(instance);
        vkDestroyInstance(instance, nullptr);
    }

    [[maybe_unused]] const VkInstance ContextInterface::getInstance() const
    {
        return instance;
    }

    [[maybe_unused]] const VkDevice ContextInterface::getDevice() const
    {
        return device;
    }

    [[maybe_unused]] const VkPhysicalDeviceProperties &ContextInterface::getProperties() const
    {
        return properties;
    }

    [[maybe_unused]] const VkSwapchainKHR ContextInterface::getSwapchain() const
    {
        return swapchain;
    }

    [[maybe_unused]] const VkPhysicalDeviceFeatures &ContextInterface::getDeviceFeatures() const
    {
        return deviceFeatures;
    }

    [[maybe_unused]] const VkSurfaceCapabilitiesKHR &ContextInterface::getSurfaceCapabilities() const
    {
        return surfaceCapabilities;
    }

    [[maybe_unused]] const VkSurfaceFormatKHR &ContextInterface::getSurfaceFormat() const
    {
        return surfaceFormat;
    }

    [[maybe_unused]] const VkQueue ContextInterface::getGraphicQueue() const
    {
        return graphicQueue;
    }

    [[maybe_unused]] uint32_t ContextInterface::getFamilyIndex() const
    {
        return family_index;
    }
}
