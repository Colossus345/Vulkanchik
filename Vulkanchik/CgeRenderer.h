#pragma once

#include"CG_window.h"
#include"CgeDevice.h"
#include"CgeSwapchain.h"



#include<cassert>
#include<memory>
#include<vector>

namespace cge {
	class CgeRenderer {

	public:

		CgeRenderer(CgeWindow& window, CgeDevice& device);
		~CgeRenderer();

		CgeRenderer(const CgeRenderer&) = delete;
		CgeRenderer& operator =(const CgeRenderer&) = delete;

		VkRenderPass getSwapChainRenderPass() const { return cgeSwapChain->getRenderPass(); }
		float getAspectRation() const { return cgeSwapChain->extentAspectRatio(); }
		bool isFrameInProgress() const { return isFrameStarted; }

		VkCommandBuffer getCurrentCommandBuffer() const {
			assert(isFrameStarted && "cannot get frame buffer when frame not in progress");
			return commandBuffers[currentFrameIndex];
		}
		int getFrameIndex() const {
			assert(isFrameStarted && "Cannot get command buffer");
			return currentFrameIndex;
		}

		VkCommandBuffer beginFrame();
		void endFrame();
		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);
	private:


		void createCommandBuffers();
		void freeCommandBuffers();
		void recreateSwapchain();

		CgeWindow& cgeWindow;
		CgeDevice& cgeDevice;
		std::unique_ptr<CgeSwapChain> cgeSwapChain;
		std::vector<VkCommandBuffer> commandBuffers;
		
		uint32_t currentImageIndex{0};
		int currentFrameIndex{0};
		bool isFrameStarted{false};


	};
}