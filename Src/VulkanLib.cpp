//
// Created by luket on 09/01/2021.
//

//TODO: write documentation
#include <array>
#include <stdexcept>
#include "Context/ContextInterface.h"
#include "VulkanLib.h"
#include "VulkanLibRenderComands.h"
#include "Context/Componants/FrameBuffer.h"

namespace LunaLuxVulkanLib
{
    bool IDebug{false};
    uint32_t currentFrame = 0;
    ContextInterface* context;
    VkRenderPass renderPass = nullptr;
    LunaLuxWindowLib::Window* window = nullptr;
    FrameBuffer* frameBuffer;

    void createContext(bool debug, LunaLuxWindowLib::Window * window_in)
    {
        window = window_in;
        if(debug) printf("WARRING: enabling full runtime debug may cause quirks that are not in non debug runtime "
                         "set(this is in the FIX ME: list)");
        IDebug = debug;
        context = new ContextInterface();
        context->createContext(debug,window);

        std::array<VkAttachmentDescription, 2> attachments {};
        attachments[ 0 ].flags						= 0;
        attachments[ 0 ].format						= context->getDepthHandler()->getDepthFormat();
        attachments[ 0 ].samples					= VK_SAMPLE_COUNT_1_BIT;
        attachments[ 0 ].loadOp						= VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachments[ 0 ].storeOp					= VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachments[ 0 ].stencilLoadOp				= VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachments[ 0 ].stencilStoreOp				= VK_ATTACHMENT_STORE_OP_STORE;
        attachments[ 0 ].initialLayout				= VK_IMAGE_LAYOUT_UNDEFINED;
        attachments[ 0 ].finalLayout				= VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        attachments[ 1 ].flags						= 0;
        attachments[ 1 ].format						= context->getSurface()->getSurfaceFormat().format;
        attachments[ 1 ].samples					= VK_SAMPLE_COUNT_1_BIT;
        attachments[ 1 ].loadOp						= VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachments[ 1 ].storeOp					= VK_ATTACHMENT_STORE_OP_STORE;
        attachments[ 1 ].initialLayout				= VK_IMAGE_LAYOUT_UNDEFINED;
        attachments[ 1 ].finalLayout				= VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;


        VkAttachmentReference sub_pass_0_depth_stencil_attachment {};
        sub_pass_0_depth_stencil_attachment.attachment	= 0;
        sub_pass_0_depth_stencil_attachment.layout		= VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        std::array<VkAttachmentReference, 1> sub_pass_0_color_attachments {};
        sub_pass_0_color_attachments[ 0 ].attachment	= 1;
        sub_pass_0_color_attachments[ 0 ].layout		= VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        std::array<VkSubpassDescription, 1> sub_passes {};
        sub_passes[ 0 ].pipelineBindPoint			= VK_PIPELINE_BIND_POINT_GRAPHICS;
        sub_passes[ 0 ].colorAttachmentCount		= sub_pass_0_color_attachments.size();
        sub_passes[ 0 ].pColorAttachments			= sub_pass_0_color_attachments.data();
        sub_passes[ 0 ].pDepthStencilAttachment		= &sub_pass_0_depth_stencil_attachment;


        VkRenderPassCreateInfo render_pass_create_info {};
        render_pass_create_info.sType				= VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        render_pass_create_info.attachmentCount		= attachments.size();
        render_pass_create_info.pAttachments		= attachments.data();
        render_pass_create_info.subpassCount		= sub_passes.size();
        render_pass_create_info.pSubpasses			= sub_passes.data();

        vkCreateRenderPass( context->getDevice()->getDev(), &render_pass_create_info, nullptr, &renderPass );

        auto[width,height] = window->GetWindowSize();
        frameBuffer = new FrameBuffer(context->getDevice(),context->getSwapChain(),renderPass,context->getDepthHandler(),width,height);
    }

    const VkDevice getDevice()
    {
        return context->getDevice()->getDev();
    }

    const VkRenderPass getRenderPass()
    {
        return renderPass;
    }

    void destroyContext()
    {
        vkQueueWaitIdle(context->getGraphicQueue());
        delete frameBuffer;
        vkDestroyRenderPass(context->getDevice()->getDev(), renderPass, nullptr );
        context->destroyContext(IDebug);
    }

    VkRenderPassBeginInfo vkClearColour(float r, float g, float b)
    {

        VkClearValue clear_values[2];
        clear_values[0].depthStencil.depth	= 1.0f;
        clear_values[0].depthStencil.stencil= 0;
        clear_values[1].color.float32[0] = r;
        clear_values[1].color.float32[1] = g;
        clear_values[1].color.float32[2] = b;
        clear_values[1].color.float32[3] = 1.0f;

        VkRenderPassBeginInfo render_pass_begin_info {};
        render_pass_begin_info.sType				= VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        render_pass_begin_info.pNext                = nullptr;
        render_pass_begin_info.renderPass			= renderPass;
        render_pass_begin_info.renderArea.offset.x  = 0;
        render_pass_begin_info.renderArea.offset.y  = 0;
        auto[width,height] = window->GetWindowSize();
        render_pass_begin_info.renderArea.extent.width = width;
        render_pass_begin_info.renderArea.extent.height = height;
        render_pass_begin_info.clearValueCount		= 2;
        render_pass_begin_info.pClearValues         = clear_values;
        render_pass_begin_info.framebuffer			= frameBuffer->getFrameBuffers()[currentFrame];

        return render_pass_begin_info;
    }

    bool frameBegin(VkFence fence)
    {
        bool change = false;
        jumpback:
        vkQueueWaitIdle(context->getGraphicQueue());
        VkResult result = vkAcquireNextImageKHR(context->getDevice()->getDev(),context->getSwapChain()->getSwapchain(),UINT64_MAX,
                                                VK_NULL_HANDLE,fence,&currentFrame );
        if(result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            if(!window->ShouldClose())
            {
                auto[width, height] = window->GetWindowSize();
                context->reset(width, height);
                frameBuffer->reset(context->getSwapChain(), renderPass, context->getDepthHandler(), width, height);
                vkResetFences(  context->getDevice()->getDev(), 1, &fence );
                change = true;
                goto jumpback;
            }
        }
        else if(result != VK_SUCCESS) printf("swapChain Error: %i\n",result);

        vkWaitForFences( context->getDevice()->getDev(), 1, &fence, VK_TRUE, UINT64_MAX );
        vkResetFences(  context->getDevice()->getDev(), 1, &fence );
        return change;
    }

    void frameSubmit(std::vector<VkSemaphore> wait_semaphores,VkSubmitInfo submitInfo)
    {
        vkQueueSubmit( context->getGraphicQueue(), 1, &submitInfo, VK_NULL_HANDLE );
        VkResult present_result = VkResult::VK_RESULT_MAX_ENUM;


        VkPresentInfoKHR present_info {};
        present_info.sType					= VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        present_info.waitSemaphoreCount		= wait_semaphores.size();
        present_info.pWaitSemaphores		= wait_semaphores.data();
        present_info.swapchainCount			= 1;
        VkSwapchainKHR swapchain = context->getSwapChain()->getSwapchain();
        present_info.pSwapchains			= &swapchain;
        present_info.pImageIndices			= &currentFrame;
        present_info.pResults				= &present_result;

        VkResult result;
        result = vkQueuePresentKHR( context->getGraphicQueue(), &present_info );
        if(result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            if(!window->ShouldClose())
            {
                auto[width, height] = window->GetWindowSize();
                context->reset(width, height);
                frameBuffer->reset(context->getSwapChain(), renderPass, context->getDepthHandler(), width, height);
            }
        }
        else if(result != VK_SUCCESS)
        {
            printf("swapChain Error: %i\n",result);
        }
    }

    VkCommandPool vkGenCommandPool(VkCommandPoolCreateFlags flags)
    {
        VkCommandPool command_pool;

        VkCommandPoolCreateInfo pool_create_info {};
        pool_create_info.sType				= VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        pool_create_info.flags				= flags;
        pool_create_info.queueFamilyIndex	= context->getDevice()->getGraphicFamilyIndex();

        vkCreateCommandPool(context->getDevice()->getDev(), &pool_create_info, nullptr, &command_pool );
        return command_pool;
    }

    uint32_t findGpuMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
    {
        return context->getDevice()->findGpuMemoryType(typeFilter,properties);
    }


    VkCommandBuffer LunaLuxVulkanLib::vkAllocateCommandBuffers(VkCommandBufferAllocateInfo * bufferAllocateInfo)
    {
        VkCommandBuffer buffer = {};
        if(vkAllocateCommandBuffers(context->getDevice()->getDev(),bufferAllocateInfo,&buffer) != VK_SUCCESS)
            throw std::runtime_error("failed to allocate vertex buffer memory!");
        return buffer;
    }

    void vkQueueWaitIdle()
    {
        vkQueueWaitIdle(context->getGraphicQueue());
    }
    VkSemaphore vkGenSemaphore()
    {
        VkSemaphore semaphore;
        VkSemaphoreCreateInfo semaphore_create_info {};
        semaphore_create_info.sType				= VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        vkCreateSemaphore( context->getDevice()->getDev(), &semaphore_create_info, nullptr, &semaphore );
        return semaphore;
    }

    VkFence vkGenFence()
    {
        VkFence fence;
        VkFenceCreateInfo fenceCreateInfo = {};
        fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceCreateInfo.pNext = nullptr;

        vkCreateFence(context->getDevice()->getDev(),&fenceCreateInfo ,nullptr,&fence);
        return fence;
    }

    VkShaderModule vkGenShaderModule(const std::vector<char>& code)
    {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shaderModule;
        VkResult result;
        if ((result = vkCreateShaderModule(context->getDevice()->getDev(), &createInfo, nullptr, &shaderModule)) != VK_SUCCESS)
            printf("shader byteCode compile error: %i\n",result);

        return shaderModule;
    }

    void vkDestroyShaderModule(VkShaderModule module)
    {
        vkDestroyShaderModule(context->getDevice()->getDev(),module, nullptr);
    }

    std::tuple<VkPipeline,VkPipelineLayout> vkGenDefaultPipeline(LunaLuxWindowLib::Window* window,VkPipelineShaderStageCreateInfo shaderStages[],
                                    VkPipelineVertexInputStateCreateInfo vertexInputInfo,VkPipelineInputAssemblyStateCreateInfo inputAssembly)
    {
        VkPipeline pipeline = nullptr;
        VkPipelineLayout pipelineLayout = {};
        auto[_width_,_height_] = window->GetWindowSize();
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float) _width_;
        viewport.height = (float) _height_;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent.width  = _width_;
        scissor.extent.height = _height_;

        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;

        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;

        VkPipelineDepthStencilStateCreateInfo depthStencil{};
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable = VK_TRUE;
        depthStencil.depthWriteEnable = VK_TRUE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.stencilTestEnable = VK_FALSE;

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pushConstantRangeCount = 0;

        if (vkCreatePipelineLayout(context->getDevice()->getDev(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
            throw std::runtime_error("failed to create pipeline layout!");

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pDepthStencilState = &depthStencil;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.layout = pipelineLayout;
        pipelineInfo.renderPass = renderPass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        if (vkCreateGraphicsPipelines(context->getDevice()->getDev(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS)
            throw std::runtime_error("failed to create graphics pipeline!");

        if(pipeline == nullptr)
            throw std::runtime_error("failed to create graphics pipeline!");
        return {pipeline,pipelineLayout};
    }

    void vkDestroyPipeline(VkPipeline pipe)
    {
        vkDestroyPipeline(context->getDevice()->getDev(),pipe, nullptr);
    }

    void vkDestroyPipelineLayout(VkPipelineLayout pipe)
    {
        vkDestroyPipelineLayout(context->getDevice()->getDev(), pipe, nullptr);
    }

    void vkDestroyFence(VkFence fence)
    {
        vkDestroyFence(context->getDevice()->getDev(),fence, nullptr);
    }

    void vkDestroySemaphore(VkSemaphore semaphore)
    {
        vkDestroySemaphore(context->getDevice()->getDev(),semaphore, nullptr);
    }

    void vkDestroyCommandPool(VkCommandPool commandPool)
    {
        vkDestroyCommandPool(context->getDevice()->getDev(),commandPool, nullptr);
    }

    VkDeviceMemory vkAllocateMemory(VkMemoryRequirements memRequirements,VkMemoryPropertyFlags flags)
    {
        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findGpuMemoryType(memRequirements.memoryTypeBits,flags);
        VkDeviceMemory BufferMemory;
        if (vkAllocateMemory(context->getDevice()->getDev(), &allocInfo, nullptr, &BufferMemory) != VK_SUCCESS)
            throw std::runtime_error("failed to allocate vertex buffer memory!");
        return BufferMemory;
    }

    VkBuffer vkGenBuffer(VkBufferCreateInfo* bufferCreateInfo)
    {
        VkBuffer buffer_ret;
        if (vkCreateBuffer(context->getDevice()->getDev(), bufferCreateInfo, nullptr, &buffer_ret) != VK_SUCCESS)
            throw std::runtime_error("failed to create vertex buffer!");
        return buffer_ret;
    }

    VkMemoryRequirements vkGetBufferMemoryRequirements(VkBuffer buffer)
    {
        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(context->getDevice()->getDev(), buffer, &memRequirements);
        return memRequirements;
    }

    void vkFreeMemory(VkDeviceMemory memory)
    {
        vkFreeMemory(context->getDevice()->getDev(),memory, nullptr);

    }

    void vkDestroyBuffer(VkBuffer buffer)
    {
        vkDestroyBuffer(context->getDevice()->getDev(),buffer, nullptr);
    }

    void vkBufferTransferData(void * data_in, VkDeviceMemory BufferMemory, VkBufferCreateInfo bufferInfo)
    {
        void* data;
        vkMapMemory(context->getDevice()->getDev(), BufferMemory, 0, bufferInfo.size, 0, &data);
        memcpy(data, data_in, (size_t) bufferInfo.size);
        vkUnmapMemory(context->getDevice()->getDev(), BufferMemory);
    }

    VkPipelineShaderStageCreateInfo vkGenShaderLink(VkShaderModule module, VkShaderStageFlagBits stageFlagBits)
    {
        VkPipelineShaderStageCreateInfo ShaderStageInfo = {};
        ShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        ShaderStageInfo.stage = stageFlagBits;
        ShaderStageInfo.module = module;
        ShaderStageInfo.pName = "main";
        return ShaderStageInfo;
    }
}
