//
// Created by luket on 09/01/2021.
//

//TODO: write documentation
#include "ContextInterface.h"
#include "../Common/Utils.h"
#include "GPU.h"
#include "Debug.h"

#include <stdexcept>
namespace LunaLuxVulkanLib
{

    void ContextInterface::createContext(bool debug, LunaLuxWindowLib::Window * window)
    {
        {
            std::vector<const char *> instextend;

            instextend.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
            if(debug) instextend.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
#ifdef LLE_WINDOWS
            instextend.push_back("VK_KHR_win32_surface");
#endif
            uint32_t l_count = 0;
            if (debug) vkEnumerateInstanceLayerProperties(&l_count, nullptr);
            std::vector<const char *> instlayname(l_count);
            std::vector<VkLayerProperties> instlay(l_count);
            if (debug)
            {
                vkEnumerateInstanceLayerProperties(&l_count, instlay.data());
                int temp = 0;
                if (debug)
                    for (auto &i: instlay)
                    {
                        instlayname[temp] = i.layerName;
                        temp++;
                    }
                {
                    auto temp_ = instlayname.back();
                    instlayname[0] = temp_;
                    instlayname.pop_back();
                }
                {
                    auto temp_ = instlayname.back();
                    instlayname[1] = temp_;
                    instlayname.pop_back();
                }
            }

            // initialize the VkApplicationInfo structure
            VkApplicationInfo app_info = {};
            app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            app_info.pNext = nullptr;
            app_info.pApplicationName = "LunaLuxVulkanLib_app";
            app_info.applicationVersion = 1;
            app_info.pEngineName = "LunaLuxVulkanLib";
            app_info.engineVersion = 1;
            app_info.apiVersion = VK_API_VERSION_1_0;
            // initialize the VkInstanceCreateInfo structure
            VkInstanceCreateInfo inst_info = {};
            inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            inst_info.pNext = nullptr;
            inst_info.flags = 0;
            inst_info.pApplicationInfo = &app_info;
            inst_info.enabledExtensionCount = instextend.size();
            inst_info.ppEnabledExtensionNames = instextend.data();
            if (debug)
            {
                inst_info.enabledLayerCount = instlayname.size();
                inst_info.ppEnabledLayerNames = instlayname.data();
            }
            else
            {
                inst_info.enabledLayerCount = 0;
                inst_info.ppEnabledLayerNames = nullptr;
            }
            debug_wrapper(debug, vkCreateInstance(&inst_info, nullptr, &instance))
        }
        if(debug) createDebug(instance);
#ifdef LLE_WINDOWS
        createWin32Surf(debug,instance,&surface,window);
#endif
        {
            VkPhysicalDevice p_device = getPDevice(instance);
            vkGetPhysicalDeviceProperties(p_device, &properties);

            float queue[]{1.0f};
            VkDeviceQueueCreateInfo queue_info = {};
            queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue_info.pNext = nullptr;
            queue_info.flags = 0;
            {
                std::vector<VkQueueFamilyProperties> list = get_vector_list(p_device);

                for (unsigned int i = 0; i < list.size(); i++)
                    if (list[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                    {
                        queue_info.queueFamilyIndex = i;
                        break;
                    }
            }
            queue_info.pQueuePriorities = queue;
            queue_info.queueCount = 1;

            uint32_t l_count = 0;
            if (debug) vkEnumerateDeviceLayerProperties(p_device,&l_count, nullptr);
            std::vector<const char *> devlayname(l_count);
            std::vector<VkLayerProperties> devlay(l_count);
            if (debug)
            {
                vkEnumerateDeviceLayerProperties(p_device,&l_count, devlay.data());
                int temp = 0;
                if (debug)
                    for (auto &i: devlay)
                    {
                        devlayname[temp] = i.layerName;
                        temp++;
                    }
            }

            VkDeviceCreateInfo deviceCreateInfo = {};
            deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            deviceCreateInfo.pNext = nullptr;
            deviceCreateInfo.flags = 0;
            deviceCreateInfo.enabledExtensionCount = 0;
            deviceCreateInfo.enabledLayerCount = devlayname.size();
            deviceCreateInfo.ppEnabledExtensionNames = nullptr;
            deviceCreateInfo.ppEnabledLayerNames = devlayname.data();
            deviceCreateInfo.pEnabledFeatures = nullptr;
            deviceCreateInfo.pQueueCreateInfos = &queue_info;
            deviceCreateInfo.queueCreateInfoCount = 1;

            debug_wrapper(debug, vkCreateDevice(p_device, &deviceCreateInfo, nullptr, &device))
        }
        {

        }
    }

    void ContextInterface::destroyContext(bool debug)
    {
        if(debug) destroyDebug(instance);
        vkDestroyDevice(device, nullptr);
        vkDestroySurfaceKHR(instance,surface, nullptr);
        vkDestroyInstance(instance, nullptr);
    }

    [[maybe_unused]] const VkInstance_T *ContextInterface::getInstance() const
    {
        return instance;
    }

    [[maybe_unused]] const VkDevice_T *ContextInterface::getDevice() const
    {
        return device;
    }

    [[maybe_unused]] const VkSurfaceKHR_T *ContextInterface::getSurface() const
    {
        return surface;
    }

    [[maybe_unused]] const VkPhysicalDeviceProperties &ContextInterface::getProperties() const
    {
        return properties;
    }

    [[maybe_unused]] const VkSwapchainKHR_T *ContextInterface::getSwapchain() const
    {
        return swapchain;
    }
}
