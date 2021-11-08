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
		SimpleRenderSystem(CgeDevice& device,VkRenderPass renderPass);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator =(const SimpleRenderSystem&) = delete;

		void renderGameObjects(FrameInfo &frameInfo, 
			std::vector<CgeGameObject>& gameObjects);

	private:
		 
		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);
		

		CgeDevice& cgeDevice;
		std::unique_ptr<CgePipeline> cgePipeline;
		VkPipelineLayout pipelineLayout;

		


	};
}