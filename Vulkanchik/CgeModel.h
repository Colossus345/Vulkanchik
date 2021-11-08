#pragma once

#include"CgeDevice.h"
#include"Cge_buffer.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include<memory>
#include<glm/glm.hpp>



namespace cge {

	class CgeModel {

	public:
		struct Vertex
		{
			glm::vec3 position;
			glm::vec3 color;
			glm::vec3 normal{};
			glm::vec2 uv{};
			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

			bool operator==(const Vertex& other)const {
				return position == other.position &&
					color == other.color &&
					normal == other.normal &&
					uv == other.uv;

			}
		};
		struct Data {
			std::vector<Vertex> vertices{};
			std::vector<uint32_t> indices{};

			void loadModel(const std::string& filename);
		};

		CgeModel(CgeDevice &device,const CgeModel::Data &data);
		~CgeModel();



		CgeModel(const CgeModel&) = delete;
		CgeModel& operator =(const CgeModel&) = delete;

		static std::unique_ptr<CgeModel> createModelFromFile(CgeDevice& device, const std::string& filepath);


		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);
	private:
		void createVertexBuffers(const std::vector<Vertex>& verticies);
		void createIndexBuffers(const std::vector<uint32_t>& indicies);


		CgeDevice &cgeDevice;

		std::unique_ptr<CgeBuffer> vertexBuffer;
		uint32_t vertexCount;

		bool hasIndexBuffer = false;

		std::unique_ptr<CgeBuffer> indexBuffer;
		uint32_t indexCount;


		
	};
}