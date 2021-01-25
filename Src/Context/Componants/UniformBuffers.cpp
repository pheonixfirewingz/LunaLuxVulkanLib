//
// Created by luket on 25/01/2021.
//

//TODO: write documentation
#include <stdexcept>
#include "UniformBuffers.h"

namespace LunaLuxVulkanLib
{

    void UniformBuffers::Create(SwapChain *swapChain,void* data_in)
    {
        VkDeviceSize size = sizeof(data_in);
        uniformBuffers.resize(swapChain->getImageCount());
        uniformMemorys.resize(swapChain->getImageCount());
        for(uint32_t i = 0; i < swapChain->getImageCount();i++)
        {
            VkBufferCreateInfo bufferInfo{};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = size;
            bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
            bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            if (vkCreateBuffer(device->getDev(), &bufferInfo, nullptr, &uniformBuffers[i]) != VK_SUCCESS)
                throw std::runtime_error("failed to create buffer!");


            VkMemoryRequirements memRequirements;
            vkGetBufferMemoryRequirements(device->getDev(), uniformBuffers[i], &memRequirements);

            VkMemoryAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memRequirements.size;
            allocInfo.memoryTypeIndex = device->findGpuMemoryType(memRequirements.memoryTypeBits,
                                                                  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                                                  VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

            if (vkAllocateMemory(device->getDev(), &allocInfo, nullptr, &uniformMemorys[i]) != VK_SUCCESS)
                throw std::runtime_error("failed to allocate buffer memory!");

            vkBindBufferMemory(device->getDev(), uniformBuffers[i], uniformMemorys[i], 0);

            void* data;
            vkMapMemory(device->getDev(), uniformMemorys[i], 0, bufferInfo.size, 0, &data);
            memcpy(data, data_in, (size_t) bufferInfo.size);
            vkUnmapMemory(device->getDev(), uniformMemorys[i]);
        }
    }

    void UniformBuffers::Destroy()
    {
        for(const auto& u:uniformBuffers)
        {
            vkDestroyBuffer(device->getDev(),uniformBuffers.emplace_back(), nullptr);
            vkFreeMemory(device->getDev(),uniformMemorys.emplace_back(), nullptr);
        }
    }

    UniformBuffers::UniformBuffers(Device *device, SwapChain *swapChain,void* data_in): device(device)
    {
        Create(swapChain,data_in);
    }

    void UniformBuffers::reset(SwapChain *swapChain,void* data_in)
    {
        Destroy();
        Create(swapChain,data_in);
    }

    UniformBuffers::~UniformBuffers()
    {
        Destroy();
    }

    const std::vector<VkBuffer> &UniformBuffers::getUniformBuffers() const
    {
        return uniformBuffers;
    }
};
