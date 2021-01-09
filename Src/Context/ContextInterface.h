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
        VkSwapchainKHR swapchain;
        VkQueue graphicQueue,presentQueue;
        std::vector<const char*> instextend;
    public:
        void createContext(bool,LunaLuxWindowLib::Window*);
        void destroyContext(bool);
    };
};
#endif //GITIGNORE_CONTEXTINTERFACE_H
