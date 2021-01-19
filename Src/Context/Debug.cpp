//
// Created by luket on 13/01/2021.
//

#include "Debug.h"
//TODO: write documentation
#include <vulkan/vulkan_core.h>
#include <cstdio>

namespace LunaLuxVulkanLib
{
    VkDebugReportCallbackEXT reportCallback = nullptr;
    PFN_vkCreateDebugReportCallbackEXT lle_vkCreateDebugReportCallback = nullptr;
    PFN_vkDestroyDebugReportCallbackEXT lle_vkDestroyDebugReportCallback = nullptr;
}

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags,VkDebugReportObjectTypeEXT type, uint64_t src,
                                             size_t location,int32_t code,const char * layer,const char * msg,void * user_data)
{
    if(flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) return false;
    printf("LunaLuxVulkanLib_debugCallback layer fired: %s\n",layer);
    if(flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) printf("warning msg:\n %s\n",msg);
    else if(flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) printf("debug msg:\n %s\n",msg);
    else if(flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) printf("performance msg:\n %s\n",msg);
    else if(flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) printf("error msg:\n %s\n", msg);
    return false;
}

void LunaLuxVulkanLib::createDebug(VkInstance instance)
{
    lle_vkCreateDebugReportCallback = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(
            instance, "vkCreateDebugReportCallbackEXT"));
    lle_vkDestroyDebugReportCallback = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr(
            instance, "vkDestroyDebugReportCallbackEXT"));
    VkDebugReportCallbackCreateInfoEXT reportCallbackCreateInfoExt = {};
    reportCallbackCreateInfoExt.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    reportCallbackCreateInfoExt.pNext = nullptr;
    reportCallbackCreateInfoExt.flags =  VK_DEBUG_REPORT_INFORMATION_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT |
                                         VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | VK_DEBUG_REPORT_ERROR_BIT_EXT |
                                         VK_DEBUG_REPORT_DEBUG_BIT_EXT | 0;
    reportCallbackCreateInfoExt.pUserData = nullptr;
    reportCallbackCreateInfoExt.pfnCallback = debugCallback;
    lle_vkCreateDebugReportCallback(instance,&reportCallbackCreateInfoExt, nullptr,&reportCallback);
}

void LunaLuxVulkanLib::destroyDebug(VkInstance instance)
{
    lle_vkDestroyDebugReportCallback(instance,reportCallback,nullptr);
    lle_vkDestroyDebugReportCallback = nullptr;
    lle_vkCreateDebugReportCallback = nullptr;
}
