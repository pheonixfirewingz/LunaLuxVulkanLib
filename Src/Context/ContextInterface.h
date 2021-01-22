//
// Created by luket on 09/01/2021.
//

#ifndef GITIGNORE_CONTEXTINTERFACE_H
#define GITIGNORE_CONTEXTINTERFACE_H
//TODO: write documentation
#include <LunaLuxWindowLib/Window.h>
#include <vulkan/vulkan_core.h>
#include <vector>
#include "Componants/Instance.h"
#include "Componants/Device.h"
#include "Componants/Surface.h"
#include "Componants/SwapChain.h"
namespace LunaLuxVulkanLib
{
    class ContextInterface
    {
    private:
        Instance* instance = nullptr;
        Device* device = nullptr;
        Surface* surface = nullptr;
        SwapChain* swapChain = nullptr;
        uint32_t ImageCount = 0;
        VkFormat depthFormat = VK_FORMAT_UNDEFINED;
        VkQueue graphicQueue = nullptr;
        VkImage depthImage = nullptr;
        VkDeviceMemory	depthImageDeviceMemory	= nullptr;
        VkImageView		depthImageView = nullptr;
        std::vector<VkImage>				swapChain_images;
        std::vector<VkImageView>			swapChain_image_views;
    public:
        void createContext(bool,LunaLuxWindowLib::Window*);
        void destroyContext(bool);

        Instance *getInstance() const;

        Device *getDevice() const;

        Surface *getSurface() const;

        VkFormat getDepthFormat() const;

        SwapChain *getSwapChain() const;

        uint32_t getImageCount() const;

        const VkQueue getGraphicQueue() const;

        const VkImage getDepthImage() const;

        const VkDeviceMemory getDepthImageDeviceMemory() const;

        const VkImageView getDepthImageView() const;

        const std::vector<VkImage> &getSwapChainImages() const;

        const std::vector<VkImageView> &getSwapChainImageViews() const;
    };
}
#endif //GITIGNORE_CONTEXTINTERFACE_H
