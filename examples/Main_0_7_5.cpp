// Created by luket on 31/12/2020.
#include <LunaLuxWindowLib/Window.h>
#include <stdexcept>
#include <fstream>
#include <array>
#include "VulkanLib.h"
#include "VulkanLibRenderComands.h"

struct vec2 {float x,y;};

struct vec3 {float x,y,z;};

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

VkPipeline Graphic_pipeline;
VkPipelineLayout pipeline_Layout;
const std::vector<Vertex> vertices = {
        {1.0f, 0.0f, 0.0f,-0.5f, -0.5f},
        {0.0f, 1.0f, 0.0f,0.5f, -0.5f},
        {0.0f, 0.0f, 1.0f,0.5f, 0.5f},
        {1.0f, 1.0f, 1.0f,-0.5f, 0.5f}
};

const std::vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0
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

void createPipeLine(LunaLuxWindowLib::Window* window);

int main()
{
    VkFence fence;
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

    createPipeLine(window);

    VkDeviceSize bufferSize_ = sizeof(vertices[0]) * vertices.size();
    VkBuffer vertexBuffer = {};
    VkDeviceMemory vertexBufferMemory = {};
    vkGenBuffer((void*)vertices.data(),bufferSize_, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();
    VkBuffer indexBuffer = {};
    VkDeviceMemory indexBufferMemory = {};
    vkGenBuffer((void*)indices.data(),bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

    while (!window->ShouldClose())
    {
        window->Update(30.0);
        if(frameBegin(fence))
        {
            vkDestroyPipeline(Graphic_pipeline);
            vkDestroyPipelineLayout(pipeline_Layout);
            createPipeLine(window);
        }
        VkCommandBufferBeginInfo command_buffer_begin_info{};
        command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        command_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info);
        VkRenderPassBeginInfo render_pass_begin_info = vkClearColour(0.5f, 0.5f, 0.5f);
        vkCmdBeginRenderPass(command_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

        auto[width,height]= window->GetWindowSize();
        vkSetViewport(command_buffer,(float)width,(float)height);
        vkSetScissor(command_buffer,(float)width,(float)height);

        vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, Graphic_pipeline);
        VkBuffer vertexBuffers[] = {vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(command_buffer, 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(command_buffer,indexBuffer,0,VK_INDEX_TYPE_UINT16);
        vkCmdDrawIndexed(command_buffer, static_cast<uint32_t>(indices.size()), 1, 0, 0,0);

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
    vkFreeMemory(vertexBufferMemory);
    vkDestroyBuffer(vertexBuffer);
    vkDestroyPipeline(Graphic_pipeline);
    vkDestroyPipelineLayout(pipeline_Layout);
    vkDestroyFence(fence);
    vkDestroySemaphore(render_complete_semaphore);
    vkDestroyCommandPool(command_pool);
    destroyContext();
    return 0;
}

void createPipeLine(LunaLuxWindowLib::Window* window)
{
    VkShaderModule vertShaderModule = vkGenShaderModule(readFile("vert.spv"));
    VkShaderModule fragShaderModule = vkGenShaderModule(readFile("frag.spv"));

    VkPipelineShaderStageCreateInfo shaderStages[] = {vkGenShaderLink(vertShaderModule,VK_SHADER_STAGE_VERTEX_BIT),
                                                      vkGenShaderLink(fragShaderModule,VK_SHADER_STAGE_FRAGMENT_BIT)};

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

    auto[graphicsPipelines_,pipelineLayout_] = vkGenDefaultPipeline(window,shaderStages,vertexInputInfo,inputAssembly);
    Graphic_pipeline = graphicsPipelines_;
    pipeline_Layout = pipelineLayout_;

    vkDestroyShaderModule(vertShaderModule);
    vkDestroyShaderModule(fragShaderModule);
}