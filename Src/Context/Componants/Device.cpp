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
        vkGetPhysicalDeviceMemoryProperties(p_device, &memProperties);
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

    Device::~Device()
    {
        vkDestroyDevice(device, nullptr);
        p_device = nullptr;
    }

    uint32_t Device::findGpuMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
    {

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
        {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
                return i;
        }

        throw std::runtime_error("failed to find suitable memory type!");
    }

    VkPhysicalDevice Device::findRightDevice(VkInstance instance)
    {
        uint32_t count = 0;
        VkPhysicalDevice* pDevice;
        vkEnumeratePhysicalDevices(instance, &count, nullptr);
        pDevice = new VkPhysicalDevice[count];
        vkEnumeratePhysicalDevices(instance, &count, pDevice);
        if (count == 1)return pDevice[0];
        else
        {
            throw std::runtime_error("multi gpu support has not been implemented");
        }
    }

    VkPhysicalDevice Device::getPDev()
    {
        return p_device;
    }

    VkDevice Device::getDev()
    {
        return device;
    }

    const VkPhysicalDeviceMemoryProperties Device::getMemProperties() const
    {
        return memProperties;
    }

    const VkPhysicalDeviceProperties Device::getProperties() const
    {
        return properties;
    }

    const VkPhysicalDeviceFeatures Device::getDeviceFeatures() const
    {
        return deviceFeatures;
    }

    uint32_t Device::getGraphicFamilyIndex() const
    {
        return graphic_family_index;
    }
};
