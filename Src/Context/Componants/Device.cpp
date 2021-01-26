//
// Created by luket on 22/01/2021.
//

//TODO: write documentation
#include <vector>
#include <stdexcept>
#include "Device.h"

namespace LunaLuxVulkanLib
{

    Device::Device(bool debug,VkInstance instance)
    {
        const char* debugLayers[] = {"VK_LAYER_KHRONOS_validation"};
        const char * devextend[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME };
        p_device = findRightDevice(instance);
        vkGetPhysicalDeviceProperties(p_device, &properties);
        vkGetPhysicalDeviceFeatures(p_device,&deviceFeatures);
        float queue[]{1.0f};
        VkDeviceQueueCreateInfo queue_info = {};
        queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_info.pNext = nullptr;
        queue_info.flags = 0;
        uint32_t count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(p_device,&count, nullptr);
        std::vector<VkQueueFamilyProperties> list(count);
        vkGetPhysicalDeviceQueueFamilyProperties(p_device,&count, list.data());

        for (int i = 0; i < list.size(); i++)
        {
            if (list[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                queue_info.queueFamilyIndex = i;
                graphic_family_index = i;
                break;
            }
        }
        queue_info.pQueuePriorities = queue;
        queue_info.queueCount = 1;

        VkDeviceCreateInfo deviceCreateInfo = {};
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.pNext = nullptr;
        deviceCreateInfo.flags = 0;
        deviceCreateInfo.enabledExtensionCount = 1;
        deviceCreateInfo.ppEnabledExtensionNames = devextend;
        if(debug)
        {
            deviceCreateInfo.enabledLayerCount = 1;
            deviceCreateInfo.ppEnabledLayerNames = debugLayers;
        }
        else
        {
            deviceCreateInfo.enabledLayerCount = 0;
            deviceCreateInfo.ppEnabledLayerNames = nullptr;
        }
        deviceCreateInfo.pEnabledFeatures = nullptr;
        deviceCreateInfo.pQueueCreateInfos = &queue_info;
        deviceCreateInfo.queueCreateInfoCount = 1;

        vkCreateDevice(p_device, &deviceCreateInfo, nullptr, &device);
    }

    uint32_t Device::findGpuMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags flags)
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(p_device, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & flags) == flags)
                return i;

        throw std::runtime_error("failed to find suitable memory type!");
    }

    [[maybe_unused]] VkSampleCountFlagBits Device::getMaxUsableSampleCount() const
    {
        VkSampleCountFlags counts = properties.limits.framebufferColorSampleCounts &
                                    properties.limits.framebufferDepthSampleCounts;

        if (counts & VK_SAMPLE_COUNT_64_BIT) return VK_SAMPLE_COUNT_64_BIT;
        else if (counts & VK_SAMPLE_COUNT_32_BIT) return VK_SAMPLE_COUNT_32_BIT;
        else if (counts & VK_SAMPLE_COUNT_16_BIT) return VK_SAMPLE_COUNT_16_BIT;
        else if (counts & VK_SAMPLE_COUNT_8_BIT) return VK_SAMPLE_COUNT_8_BIT;
        else if (counts & VK_SAMPLE_COUNT_4_BIT) return VK_SAMPLE_COUNT_4_BIT;
        else if (counts & VK_SAMPLE_COUNT_2_BIT) return VK_SAMPLE_COUNT_2_BIT;
        else return VK_SAMPLE_COUNT_1_BIT;
    }

    VkPhysicalDevice Device::findRightDevice(VkInstance instance)
    {
        uint32_t count = 0;

        std::vector<VkPhysicalDevice> pDevice;
        vkEnumeratePhysicalDevices(instance, &count, nullptr);
        pDevice.resize(count);
        vkEnumeratePhysicalDevices(instance, &count, pDevice.data());
        //optimization for non multi gpu devices
        if (count == 1)return pDevice[0];

        std::vector<int> score(count);
        {
            int loop = 0;
            for (VkPhysicalDevice physicalDevice:pDevice)
            {
                VkPhysicalDeviceProperties deviceProperties;
                VkPhysicalDeviceMemoryProperties deviceMemoryProperties;
                vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
                vkGetPhysicalDeviceMemoryProperties(physicalDevice, &deviceMemoryProperties);
                switch (deviceProperties.deviceType)
                {
                    case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:score.at(loop) += 1;
                        break;
                    case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:score.at(loop) += 5;
                    default:score.at(loop) -= 5;
                }

                score.at(loop) = +(deviceMemoryProperties.memoryHeapCount + deviceMemoryProperties.memoryTypeCount);
                loop++;
            }
        }
        int high_score = 0,high_score_at = 0,loop = 0;
        for(int i:score)
        {
            if (i > high_score)
            {
                high_score = i;
                high_score_at = loop;
            }
            loop++;
        }
        return pDevice[high_score_at];
    }

    Device::~Device()
    {
        vkDestroyDevice(device, nullptr);
        p_device = nullptr;
    }

    VkPhysicalDevice Device::getPDev()
    {
        return p_device;
    }

    VkDevice Device::getDev()
    {
        return device;
    }

    [[maybe_unused]] VkPhysicalDeviceProperties Device::getProperties() const
    {
        return properties;
    }

    [[maybe_unused]] VkPhysicalDeviceFeatures Device::getDeviceFeatures() const
    {
        return deviceFeatures;
    }

    uint32_t Device::getGraphicFamilyIndex() const
    {
        return graphic_family_index;
    }
};
