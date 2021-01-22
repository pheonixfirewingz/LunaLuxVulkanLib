//
// Created by luket on 09/01/2021.
//

#ifndef GITIGNORE_VULKANLIB_H
#define GITIGNORE_VULKANLIB_H
//TODO: write documentation
#include <LunaLuxWindowLib/Window.h>
#include <vulkan/vulkan_core.h>
#include <vector>
namespace LunaLuxVulkanLib
{
    void createContext(bool,LunaLuxWindowLib::Window*);

    void frameBegin(VkFence);

    void frameSubmit(std::vector<VkSemaphore>,VkSubmitInfo);

    uint32_t findGpuMemoryType(uint32_t, VkMemoryPropertyFlags);

    [[maybe_unused]] [[nodiscard]] const VkDevice getDevice();

    [[maybe_unused]] [[nodiscard]] const VkRenderPass getRenderPass();

    void destroyContext();
}
using namespace LunaLuxVulkanLib;
#endif //GITIGNORE_VULKANLIB_H
