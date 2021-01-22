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
#include "Componants/DepthHandler.h"
namespace LunaLuxVulkanLib
{
    class ContextInterface
    {
    private:
        Instance* instance = nullptr;
        Device* device = nullptr;
        Surface* surface = nullptr;
        SwapChain* swapChain = nullptr;
        DepthHandler* depthHandler = nullptr;
        VkQueue graphicQueue = nullptr;

    public:
        void createContext(bool,LunaLuxWindowLib::Window*);
        void reset(uint32_t width,uint32_t height);
        void destroyContext(bool);

        Instance *getInstance() const;

        Device *getDevice() const;

        Surface *getSurface() const;

        SwapChain *getSwapChain() const;

        DepthHandler *getDepthHandler() const;

        const VkQueue getGraphicQueue() const;
    };
}
#endif //GITIGNORE_CONTEXTINTERFACE_H
