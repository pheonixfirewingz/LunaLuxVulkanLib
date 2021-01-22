//
// Created by luket on 09/01/2021.
//

//TODO: write documentation
#include "ContextInterface.h"
#include "Debug.h"

#include <stdexcept>

namespace LunaLuxVulkanLib
{

    void ContextInterface::createContext(bool debug, LunaLuxWindowLib::Window *window)
    {
        instance = new Instance(debug);
        device = new Device(debug, instance->get());
        if (debug)createDebug(instance->get());
        vkGetDeviceQueue(device->getDev(), device->getGraphicFamilyIndex(), 0, &graphicQueue);
        surface = new Surface(instance->get(), device, window);
        auto[width, height] = window->GetWindowSize();
        swapChain = new SwapChain(surface, device, width, height);
        depthHandler = new DepthHandler(device,width,height);
    }


    void ContextInterface::reset(uint32_t width, uint32_t height)
    {
        swapChain->reset(surface,width,height);
        depthHandler->reset(width,height);
    }

    void ContextInterface::destroyContext(bool debug)
    {
        delete depthHandler;
        delete swapChain;
        delete surface;
        delete device;
        if (debug) destroyDebug(instance->get());
        delete instance;
    }

    Instance *ContextInterface::getInstance() const
    {
        return instance;
    }

    Device *ContextInterface::getDevice() const
    {
        return device;
    }

    Surface *ContextInterface::getSurface() const
    {
        return surface;
    }

    SwapChain *ContextInterface::getSwapChain() const
    {
        return swapChain;
    }

    DepthHandler *ContextInterface::getDepthHandler() const
    {
        return depthHandler;
    }

    const VkQueue ContextInterface::getGraphicQueue() const
    {
        return graphicQueue;
    }
}