#include "CgeRenderer.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include<glm/glm.hpp>
#include<glm/gtc/constants.hpp>

#include<stdexcept>
#include<array>
#include<cassert>



namespace cge {

	
	
	CgeRenderer::CgeRenderer(CgeWindow& window, CgeDevice& device) : cgeWindow{ window }, cgeDevice{ device }
	{
		recreateSwapchain();
		createCommandBuffers();
	}

	CgeRenderer::~CgeRenderer()
	{
		freeCommandBuffers();
	}


	


	VkCommandBuffer CgeRenderer::beginFrame()
	{
		assert(!isFrameStarted && "cant call beginFrame while alredy in progress");
		uint32_t imageIndex;
		auto result = cgeSwapChain->acquireNextImage(&imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapchain();
			return nullptr;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed  to acquire swapchain image");
		}

		isFrameStarted = true;
		
		auto commandBuffer = getCurrentCommandBuffer();

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to record begin info");
		}
		return commandBuffer;
	}

	void CgeRenderer::endFrame()
	{
		assert(isFrameStarted && "cant call endFrame");
		auto commandBuffer = getCurrentCommandBuffer();

		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer");
		}
		auto result = cgeSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || cgeWindow.wasWindowResized()) {
			cgeWindow.resetWindowResized();
			recreateSwapchain();
			
		}
		else if (result != VK_SUCCESS) {
			throw std::runtime_error("failed  to present swapchain image");
		}

		isFrameStarted = false;

		currentFrameIndex = (currentFrameIndex + 1) % CgeSwapChain::MAX_FRAMES_IN_FLIGHT;


	}

	void CgeRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(isFrameStarted && "cant call beginswapchain");
		assert(commandBuffer == getCurrentCommandBuffer()
			&& "cant begin render pass");
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = cgeSwapChain->getRenderPass();
		renderPassInfo.framebuffer = cgeSwapChain->getFrameBuffer(currentImageIndex);

		renderPassInfo.renderArea.offset = { 0,0 };
		renderPassInfo.renderArea.extent = cgeSwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.01f,0.01f,0.01f,1.0f };
		clearValues[1].depthStencil = { 1.0f,0 };

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(cgeSwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(cgeSwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, cgeSwapChain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}

	void CgeRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(isFrameStarted && "cant call endswapchain");
		assert(commandBuffer == getCurrentCommandBuffer()
			&& "cant end render pass");
		vkCmdEndRenderPass(commandBuffer);
	}

	void CgeRenderer::createCommandBuffers()
	{
		commandBuffers.resize(CgeSwapChain::MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = cgeDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(cgeDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("Failed allocate command buffer");
		}



	}

	void CgeRenderer::freeCommandBuffers()
	{
		vkFreeCommandBuffers(cgeDevice.device(),
			cgeDevice.getCommandPool(),
			static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());

		commandBuffers.clear();
	}


	void CgeRenderer::recreateSwapchain()
	{
		auto extend = cgeWindow.getExtend();
		while (extend.width == 0 || extend.height == 0)
		{
			extend = cgeWindow.getExtend();
			glfwWaitEvents();

		}

		vkDeviceWaitIdle(cgeDevice.device());
		if (cgeSwapChain == nullptr) {
			cgeSwapChain = std::make_unique<CgeSwapChain>(cgeDevice, extend);
		}
		else {
			std::shared_ptr<CgeSwapChain> oldSwapChain=std::move(cgeSwapChain);
			cgeSwapChain = std::make_unique<CgeSwapChain>(cgeDevice, extend, oldSwapChain);

			if (!oldSwapChain->compareSwapFormats(*cgeSwapChain.get())) {
				throw std::runtime_error("swap chain image format change");
			}
		
		}
		//createPipeline();

	}

	

	


}
