#pragma once

#include"CgeDevice.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include<glm/glm.hpp>



namespace cge {

	class CgeModel {

	public:
		struct Vertex
		{
			glm::vec3 position;
			glm::vec3 color;
			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
		};

		CgeModel(CgeDevice &device,const std::vector<Vertex> &verticies);
		~CgeModel();

		CgeModel(const CgeModel&) = delete;
		CgeModel& operator =(const CgeModel&) = delete;

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);
	private:
		void createVertexBuffers(const std::vector<Vertex>& verticies);


		CgeDevice &cgeDevice;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		uint32_t vertexCount;

		
	};
}