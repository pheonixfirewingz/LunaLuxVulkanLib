//
// Created by luket on 22/01/2021.
//

#ifndef GITIGNORE_DEPTHHANDLER_H
#define GITIGNORE_DEPTHHANDLER_H
//TODO: write documentation
#include <vulkan/vulkan_core.h>
#include "Device.h"
namespace LunaLuxVulkanLib
{
    class DepthHandler
    {
    private:
        Device* device = nullptr;
        VkFormat depthFormat = VK_FORMAT_UNDEFINED;
        VkImage depthImage = nullptr;
        VkDeviceMemory	depthImageDeviceMemory	= nullptr;
        VkImageView		depthImageView = nullptr;
        bool stencilAvailable = false;
        void create(uint32_t width,uint32_t height);
        void Destroy();
    public:
        DepthHandler(Device*,uint32_t width,uint32_t height);

        void reset(uint32_t width,uint32_t height);

        ~DepthHandler();

        VkFormat getDepthFormat() const;

        const VkImageView getDepthImageView() const;

        bool isStencilAvailable() const;
    };
};
#endif //GITIGNORE_DEPTHHANDLER_H
