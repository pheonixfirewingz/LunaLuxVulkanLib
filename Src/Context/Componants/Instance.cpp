//
// Created by luket on 22/01/2021.
//
#include "Instance.h"
#include "../Interface/Platform.h"
#include <vector>
namespace LunaLuxVulkanLib
{

    //this is the Instance class constructor this is called on class creation
    Instance::Instance(bool debug)
    {
        //this const character pointer C array is used to store the text form of the debug layers used
        const char* debugLayers[] = {"VK_LAYER_KHRONOS_validation"};
        //this const charater pointer C array is used to store the text for required instance extensions
        const char* extensions[] =
                {
                        VK_KHR_SURFACE_EXTENSION_NAME,
                        "VK_KHR_win32_surface",
                        VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
                };

        //this is the struct used to define the library application for the vulkan instance to read and identify use in
        //the vulkan driver
        VkApplicationInfo app_info = {};
        app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        app_info.pNext = nullptr;
        app_info.pApplicationName = "LunaLuxVulkanLib_app";
        app_info.applicationVersion = 1;
        app_info.pEngineName = "LunaLuxVulkanLib";
        app_info.engineVersion = 1;
        app_info.apiVersion = VK_API_VERSION_1_2;

        //this is the struct used to tell the vulkan driver that we want a VkInstance and how we tell the debug layers
        //to active in our case we only will active the sdk validation layer to make sure the user compiles with the
        //vulkan specification
        VkInstanceCreateInfo inst_info = {};
        inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        inst_info.pNext = nullptr;
        inst_info.flags = 0;
        inst_info.pApplicationInfo = &app_info;
        inst_info.enabledExtensionCount = 3;
        inst_info.ppEnabledExtensionNames = extensions;
        //if the debug is enabled active debug layers
        if(debug)
        {
            inst_info.enabledLayerCount = 1;
            inst_info.ppEnabledLayerNames = debugLayers;
        }
        //else set to zero and don't sent any text layers names
        else
        {
            inst_info.enabledLayerCount = 0;
            inst_info.ppEnabledLayerNames = nullptr;
        }

        //this function asked the driver for an instance it takes in our instance struct and returns a VkInstance
        //memory pointer
        vkCreateInstance(&inst_info, 0, &instance);
    }

    //WARNING: do not delete the instance class before anything else this must be the last thing cleaned
    //this is the instance class de-constructor it is called when the Instance class is deleted
    Instance::~Instance()
    {
        //this function asked the driver to remove or instance from memory as we do not need it any more.
        vkDestroyInstance(instance, nullptr);
    }

    VkInstance Instance::get()
    {
        //returns instance memory
        return instance;
    }
};
