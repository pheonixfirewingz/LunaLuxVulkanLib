//
// Created by luket on 31/12/2020.
//
#include <LunaLuxWindowLib/Window.h>
#include <stdexcept>
#include <fstream>
#include <array>
#include "VulkanLib.h"
#include "VulkanLibRenderComands.h"

struct vec2
{
    float x,y;
};

struct vec3
{
    float x,y,z;
};

struct Vertex
{
    vec3 color;
    vec2 pos;
public:
    static VkVertexInputBindingDescription getBindingDescription()
    {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }
};

const std::vector<Vertex> vertices =
{
        { 1.0f, 0.0f, 0.0f,-0.5f, 0.5f},
        { 0.0f, 1.0f, 0.0f,-0.5f, -0.5f},
        { 0.0f, 0.0f, 1.0f,0.5f, -0.5f},
        { 0.0f, 1.0f, 0.0f,0.5f, -0.5f},
        { 0.0f, 0.0f, 0.0f,0.5f, 0.5f},
        { 1.0f, 0.0f, 1.0f,-0.5f, 0.5f}
};

static std::vector<char> readFile(const std::string& filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) throw std::runtime_error("failed to open file!");

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

int main()
{
    VkFence fence = {};
    auto* window = new LunaLuxWindowLib::Window();
    window->Open("Vulkan Library Test 2",NULL,NULL);
    createContext(false,window);

    VkCommandPool command_pool = vkGenCommandPool(VK_COMMAND_POOL_CREATE_TRANSIENT_BIT |
                                                       VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    fence = vkGenFence();

    VkCommandBufferAllocateInfo	command_buffer_allocate_info {};
    command_buffer_allocate_info.sType				= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    command_buffer_allocate_info.commandPool		= command_pool;
    command_buffer_allocate_info.level				= VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    command_buffer_allocate_info.commandBufferCount	= 1;
    VkCommandBuffer command_buffer = vkAllocateCommandBuffers(&command_buffer_allocate_info);

    VkSemaphore render_complete_semaphore = vkGenSemaphore();

    VkShaderModule vertShaderModule = vkGenShaderModule(readFile("vert.spv"));
    VkShaderModule fragShaderModule = vkGenShaderModule(readFile("frag.spv"));

    VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    auto bindingDescription = Vertex::getBindingDescription();
    VkVertexInputAttributeDescription attributeDescriptions[2];

    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, pos);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);


    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = 2;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions;


    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    auto[graphicsPipelines,pipelineLayout] = vkGenDefaultPipeline(window,shaderStages,vertexInputInfo,inputAssembly);

    vkDestroyShaderModule(vertShaderModule);
    vkDestroyShaderModule(fragShaderModule);

    VkDeviceMemory vertexBufferMemory;

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = sizeof(vertices[0]) * vertices.size();
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    VkBuffer vertexBuffer = {};

    if (vkCreateBuffer(getDevice(), &bufferInfo, nullptr, &vertexBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create vertex buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(getDevice(), vertexBuffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findGpuMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                                VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if (vkAllocateMemory(getDevice(), &allocInfo, nullptr, &vertexBufferMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate vertex buffer memory!");
    }

    vkBindBufferMemory(getDevice(), vertexBuffer, vertexBufferMemory, 0);

    void* data;
    vkMapMemory(getDevice(), vertexBufferMemory, 0, bufferInfo.size, 0, &data);
    memcpy(data, vertices.data(), (size_t) bufferInfo.size);
    vkUnmapMemory(getDevice(), vertexBufferMemory);

    vkQueueWaitIdle();
    while (!window->ShouldClose())
    {
        window->Update(30.0);
        frameBegin(fence);

        VkCommandBufferBeginInfo command_buffer_begin_info{};
        command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        command_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info);
        VkRenderPassBeginInfo render_pass_begin_info = vkClearColour(0.0f, 0.0f, 0.0f);
        vkCmdBeginRenderPass(command_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

        auto[width,height]= window->GetWindowSize();
        vkSetViewport(command_buffer,(float)width,(float)height);
        vkSetScissor(command_buffer,(float)width,(float)height);

        vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipelines);
        VkBuffer vertexBuffers[] = {vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(command_buffer, 0, 1, vertexBuffers, offsets);
        vkCmdDraw(command_buffer, static_cast<uint32_t>(vertices.size()), 1, 0, 0);

        vkCmdEndRenderPass(command_buffer);
        vkEndCommandBuffer(command_buffer);

        VkSubmitInfo submit_info{};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.waitSemaphoreCount = 0;
        submit_info.pWaitSemaphores = nullptr;
        submit_info.pWaitDstStageMask = nullptr;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &command_buffer;
        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = &render_complete_semaphore;
        frameSubmit({render_complete_semaphore}, submit_info);
    }
    vkQueueWaitIdle();
    vkFreeMemory(getDevice(),vertexBufferMemory, nullptr);
    vkDestroyBuffer(getDevice(),vertexBuffer, nullptr);
    vkDestroyPipeline(graphicsPipelines);
    vkDestroyPipelineLayout(pipelineLayout);
    vkDestroyFence(fence);
    vkDestroySemaphore(render_complete_semaphore);
    vkDestroyCommandPool(command_pool);
    destroyContext();
    return 0;
}