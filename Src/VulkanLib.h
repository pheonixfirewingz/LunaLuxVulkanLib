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
    void vkCreateContext(bool,LunaLuxWindowLib::Window*,void *);

    bool vkFrameBegin(VkFence);

    void vkFrameSubmit(std::vector<VkSemaphore>,VkSubmitInfo);

    [[maybe_unused]] [[nodiscard]] VkDevice vkGetDevice();

    [[maybe_unused]] void vkResetUniformObjectType(void*);

    [[maybe_unused]] [[nodiscard]] uint32_t vkGetFrameBufferCount();

    [[maybe_unused]] [[nodiscard]] uint32_t vkGetCurrentFrame();

    void vkDestroyContext();
}
using namespace LunaLuxVulkanLib;
#endif //GITIGNORE_VULKANLIB_H
