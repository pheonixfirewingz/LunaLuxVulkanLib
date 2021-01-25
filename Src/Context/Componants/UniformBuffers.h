//
// Created by luket on 25/01/2021.
//

#ifndef GITIGNORE_UNIFORMBUFFERS_H
#define GITIGNORE_UNIFORMBUFFERS_H
//TODO: write documentation
#include <vulkan/vulkan_core.h>
#include "Device.h"
#include "SwapChain.h"
namespace LunaLuxVulkanLib
{
    class UniformBuffers
    {
        Device* device = nullptr;
        std::vector<VkBuffer> uniformBuffers;
        std::vector<VkDeviceMemory> uniformMemorys;
        void Create(SwapChain* swapChain,void* data_in);
        void Destroy();
    public:
        UniformBuffers(Device * device,SwapChain* swapChain,void* data_in);
        void reset(SwapChain* swapChain,void* data_in);
        ~UniformBuffers();

        const std::vector<VkBuffer> &getUniformBuffers() const;
    };
};
#endif //GITIGNORE_UNIFORMBUFFERS_H
