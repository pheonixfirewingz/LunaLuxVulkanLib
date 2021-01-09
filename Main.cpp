//
// Created by luket on 31/12/2020.
//
#define WIN32_LEAN_AND_MEAN
#include <LunaLuxWindowLib/Window.h>
#include "VulkanLib.h"
float OurVertices[] = {0.0f, 0.5f, 0.0f,0.45f, -0.5, 0.0f,-0.45f, -0.5f, 0.0f,};

int main()
{
    LunaLuxWindowLib::Window* window = new LunaLuxWindowLib::Window();
    window->Open("Vulkan Library Test",NULL,NULL);

    LunaLuxVulkanLib::createContext(true,window);

    while (!window->ShouldClose())
    {
        window->Update(30.0);
    }

    LunaLuxVulkanLib::destroyContext();

    return 0;
}



