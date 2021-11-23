#pragma once

#include"CgeCamera.h"
#include"CgeDevice.h"
#include"CgeGameObject.h"
#include"CG_pipeline.h"
#include"Cge_Frame_Info.h"

#include<memory>
#include<vector>

namespace cge {
	class SimpleRenderSystem {
	public:
		SimpleRenderSystem(CgeDevice& device,VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator =(const SimpleRenderSystem&) = delete;

		void renderGameObjects(FrameInfo &frameInfo);

	private:
		 
		void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void createPipeline(VkRenderPass renderPass);
		

		CgeDevice& cgeDevice;
		std::unique_ptr<CgePipeline> cgePipeline;
		VkPipelineLayout pipelineLayout;

		


	};
}