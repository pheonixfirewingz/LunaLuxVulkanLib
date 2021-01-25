// Created by luket on 31/12/2020.
#include <LunaLuxWindowLib/Window.h>
#include <stdexcept>
#include <fstream>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#include "VulkanLib.h"
#include "VulkanLibRenderComands.h"

struct UniformBufferObject
{
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

struct Vertex
{
    glm::vec2 pos;
    glm::vec3 color;
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
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
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
VkDescriptorSetLayout descriptorSetLayout;
int main()
{

    auto* window = new LunaLuxWindowLib::Window();
    window->Open("Vulkan Library Test 2",NULL,NULL);
    auto[_width_,_height_] = window->GetWindowSize();
    UniformBufferObject ubo{};
    {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.proj = glm::perspective(glm::radians(45.0f), ((float)_width_ / (float) _height_), 0.1f, 10.0f);
        ubo.proj[1][1] *= -1;
    }
    VkFence fence;
    vkCreateContext(false,window,(void*)&ubo);

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

    VkDeviceSize bufferSize_ = sizeof(vertices[0]) * vertices.size();
    VkBuffer vertexBuffer = {};
    VkDeviceMemory vertexBufferMemory = {};
    vkGenBuffer((void*)vertices.data(),bufferSize_, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();
    VkBuffer indexBuffer = {};
    VkDeviceMemory indexBufferMemory = {};
    vkGenBuffer((void*)indices.data(),bufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.pImmutableSamplers = nullptr;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &uboLayoutBinding;

    if (vkCreateDescriptorSetLayout(vkGetDevice(), &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
        throw std::runtime_error("failed to create descriptor set layout!");

    VkDescriptorPool descriptorPool;
    vkGenUniformDescriptorPool(descriptorPool,1,vkGetFrameBufferCount());

    std::vector<VkDescriptorSetLayout> layouts(vkGetFrameBufferCount(), descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(vkGetFrameBufferCount());
    allocInfo.pSetLayouts = layouts.data();

    std::vector<VkDescriptorSet> descriptorSets(vkGetFrameBufferCount());
    if (vkAllocateDescriptorSets(vkGetDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS)
        throw std::runtime_error("failed to allocate descriptor sets!");

    for (size_t i = 0; i < vkGetFrameBufferCount(); i++)
    {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = vkGetUnifromBuffers()[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = descriptorSets[i];
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pBufferInfo = &bufferInfo;

        vkUpdateDescriptorSets(vkGetDevice(), 1, &descriptorWrite, 0, nullptr);
    }

    createPipeLine(window);

    while (!window->ShouldClose())
    {
        window->Update(30.0);
        if(vkFrameBegin(fence))
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
        vkCmdBindDescriptorSets(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_Layout,
                                0, 1,&descriptorSets[vkGetCurrentFrame()], 0, nullptr);
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
        vkFrameSubmit({render_complete_semaphore}, submit_info);
    }
    vkQueueWaitIdle();
    vkDestroyPipeline(Graphic_pipeline);
    vkDestroyPipelineLayout(pipeline_Layout);
    vkDestroyDescriptorPool(vkGetDevice(),descriptorPool, nullptr);
    vkDestroyDescriptorSetLayout(vkGetDevice(),descriptorSetLayout, nullptr);
    vkDestroyBuffer(vertexBuffer,vertexBufferMemory);
    vkDestroyBuffer(indexBuffer,indexBufferMemory);
    vkDestroyFence(fence);
    vkDestroySemaphore(render_complete_semaphore);
    vkDestroyCommandPool(command_pool);
    vkDestroyContext();
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

    auto[graphicsPipelines_,pipelineLayout_] = vkGenDefaultPipeline(window,shaderStages,vertexInputInfo,inputAssembly,1,&descriptorSetLayout);
    Graphic_pipeline = graphicsPipelines_;
    pipeline_Layout = pipelineLayout_;

    vkDestroyShaderModule(vertShaderModule);
    vkDestroyShaderModule(fragShaderModule);
}