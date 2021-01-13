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
        VkInstance instance;
        VkDevice device;
        VkSurfaceKHR  surface;
        VkPhysicalDeviceProperties properties;
        VkSwapchainKHR swapchain;
        VkQueue graphicQueue,presentQueue;
    public:
        void createContext(bool,LunaLuxWindowLib::Window*);
        void destroyContext(bool);

        [[maybe_unused]] [[nodiscard]] const VkInstance_T *getInstance() const;

        [[maybe_unused]] [[nodiscard]] const VkDevice_T *getDevice() const;

        [[maybe_unused]] [[nodiscard]] const VkSurfaceKHR_T *getSurface() const;

        [[maybe_unused]] [[nodiscard]] const VkPhysicalDeviceProperties &getProperties() const;

        [[maybe_unused]] [[nodiscard]] const VkSwapchainKHR_T *getSwapchain() const;
    };
};
#endif //GITIGNORE_CONTEXTINTERFACE_H
