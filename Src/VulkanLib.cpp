//
// Created by luket on 09/01/2021.
//

//TODO: write documentation
#include <Context/ContextInterface.h>
#include "VulkanLib.h"

namespace LunaLuxVulkanLib
{
    bool IDebug{false};
    ContextInterface* context;

    void createContext(bool debug, LunaLuxWindowLib::Window * window)
    {
        IDebug = debug;
        context = new ContextInterface();
        context->createContext(debug,window);
    }

    void destroyContext()
    {
        context->destroyContext(IDebug);
    }
};
