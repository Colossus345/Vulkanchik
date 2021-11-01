#include "Simple_render_System.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include<glm/glm.hpp>
#include<glm/gtc/constants.hpp>

#include<stdexcept>
#include<array>
#include<cassert>



namespace cge {

	struct SimplePushConstantData {
		glm::mat4 transform{ 1.f };
		
		glm::mat4 normalMatrix{ 1.f };
	};

	SimpleRenderSystem::SimpleRenderSystem(CgeDevice& device,VkRenderPass renderPass):cgeDevice{device}
	{

		createPipelineLayout();
		createPipeline(renderPass);
	}

	SimpleRenderSystem::~SimpleRenderSystem()
	{
		vkDestroyPipelineLayout(cgeDevice.device(), pipelineLayout, nullptr);
	}


	void SimpleRenderSystem::createPipelineLayout()
	{

		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);


		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

		if (vkCreatePipelineLayout(cgeDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed pipeline layout");
		}
	}
	void SimpleRenderSystem::createPipeline(VkRenderPass renderPass)
	{

		assert(pipelineLayout != nullptr && "cannot create pipeline before pipelinelayout");

		PipelineConfigInfo  pipelineConfig{};
		CgePipeline::defaultPipelineConfigInfo(pipelineConfig);

		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = pipelineLayout;
		cgePipeline = std::make_unique<CgePipeline>(
			cgeDevice,
			"shaders/simple_shader.vert.spv",
			"shaders/simple_shader.frag.spv",
			pipelineConfig);

	}


	void SimpleRenderSystem::renderGameObjects(VkCommandBuffer commandBuffer,std::vector<CgeGameObject>& gameObjects,const CgeCamera& camera)
	{
		cgePipeline->bind(commandBuffer);

		auto projectionView = camera.getProjection() * camera.getView();

		for (auto& obj : gameObjects) {

			
			SimplePushConstantData push{};
			auto modelMatrix = obj.transform.mat4();
			push.transform = projectionView * modelMatrix;
			push.normalMatrix = obj.transform.normalMatrix();

			vkCmdPushConstants(commandBuffer, pipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(SimplePushConstantData), &push);

			obj.model->bind(commandBuffer);
			obj.model->draw(commandBuffer);
		}
	}


}
