//
// Created by luket on 09/01/2021.
//

#ifndef GITIGNORE_CONTEXTINTERFACE_H
#define GITIGNORE_CONTEXTINTERFACE_H
//TODO: write documentation
#include "Interface/Platform.h"
#include <LunaLuxWindowLib/Window.h>
#include <vulkan/vulkan.h>
#include <vector>
namespace LunaLuxVulkanLib
{
    class ContextInterface
    {
    private:
        VkInstance instance = nullptr;
        VkPhysicalDevice p_device = nullptr;
        VkDevice device = nullptr;
        VkSurfaceKHR  surface = nullptr;
        VkPhysicalDeviceProperties properties;
        VkPhysicalDeviceFeatures deviceFeatures;
        VkSurfaceCapabilitiesKHR surfaceCapabilities;
        VkSurfaceFormatKHR surfaceFormat;
        uint32_t family_index = 0;
    public:


    private:

        VkSwapchainKHR swapchain = nullptr;
        VkQueue graphicQueue = nullptr;
        std::vector<VkImage>				swapChain_images;
        std::vector<VkImageView>			swapChain_image_views;
    public:
        void createContext(bool,LunaLuxWindowLib::Window*);
        void updateContext(bool,LunaLuxWindowLib::Window*);
        void destroyContext(bool);
        [[maybe_unused]] [[nodiscard]] uint32_t getFamilyIndex() const;

        [[maybe_unused]] [[nodiscard]] const VkQueue getGraphicQueue() const;

        [[maybe_unused]] [[nodiscard]] const VkInstance getInstance() const;

        [[maybe_unused]] [[nodiscard]] const VkDevice getDevice() const;

        [[maybe_unused]] [[nodiscard]] const VkPhysicalDeviceProperties &getProperties() const;

        [[maybe_unused]] [[nodiscard]] const VkPhysicalDeviceFeatures &getDeviceFeatures() const;

        [[maybe_unused]] [[nodiscard]] const VkSurfaceCapabilitiesKHR &getSurfaceCapabilities() const;

        [[maybe_unused]] [[nodiscard]] const VkSurfaceFormatKHR &getSurfaceFormat() const;

        [[maybe_unused]] [[nodiscard]] const VkSwapchainKHR getSwapchain() const;
    };
}
#endif //GITIGNORE_CONTEXTINTERFACE_H
