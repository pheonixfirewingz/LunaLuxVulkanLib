//
// Created by luket on 22/01/2021.
//

#ifndef GITIGNORE_DEVICE_H
#define GITIGNORE_DEVICE_H
//TODO: write documentation
#include <vulkan/vulkan_core.h>
namespace LunaLuxVulkanLib
{
    class Device
    {
    private:
        VkPhysicalDevice p_device = nullptr;
        VkDevice device = nullptr;
        VkPhysicalDeviceProperties properties;
        VkPhysicalDeviceFeatures deviceFeatures;
        uint32_t graphic_family_index = 0;
        static VkPhysicalDevice findRightDevice(VkInstance);
    public:
        Device(bool debug,VkInstance instance);

        ~Device();

        uint32_t findGpuMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags flags);

        VkPhysicalDevice getPDev();

        VkDevice getDev();

        uint32_t getGraphicFamilyIndex() const;

        [[maybe_unused]] VkSampleCountFlagBits getMaxUsableSampleCount() const;

        [[maybe_unused]] VkPhysicalDeviceProperties getProperties() const;

        [[maybe_unused]] VkPhysicalDeviceFeatures getDeviceFeatures() const;
    };
};
#endif //GITIGNORE_DEVICE_H
