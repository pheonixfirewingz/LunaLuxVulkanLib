//
// Created by luket on 09/01/2021.
//

//TODO: write documentation
#include "VulkanLib.h"

namespace LunaLuxVulkanLib
{
    bool IDebug{false};

    void createContext(bool debug, LunaLuxWindowLib::Window * window)
    {
        IDebug = debug;
    }

    void destroyContext()
    {

    }
};
