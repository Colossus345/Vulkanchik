#include "CgeModel.h"

namespace cge {
	CgeModel::CgeModel(CgeDevice& device, const std::vector<Vertex>& verticies) :cgeDevice{ device } {
		createVertexBuffers(verticies);
	}

	CgeModel::~CgeModel() {
		vkDestroyBuffer(cgeDevice.device(), vertexBuffer, nullptr);
		vkFreeMemory(cgeDevice.device(), vertexBufferMemory, nullptr);

	}

	void CgeModel::bind(VkCommandBuffer commandBuffer)
	{
		VkBuffer buffers[] = { vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

	}

	void CgeModel::draw(VkCommandBuffer commandBuffer)
	{
		vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
	}

	void CgeModel::createVertexBuffers(const std::vector<Vertex>& verticies)
	{
		vertexCount = static_cast<uint32_t>(verticies.size());
		assert(vertexCount >= 3 && "vertex must be at least 3");
		VkDeviceSize bufferSize = sizeof(verticies[0]) * vertexCount;
		cgeDevice.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			vertexBuffer,
			vertexBufferMemory
		);

		void* data;
		vkMapMemory(cgeDevice.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, verticies.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(cgeDevice.device(), vertexBufferMemory);




	}

	
	std::vector<VkVertexInputBindingDescription> CgeModel::Vertex::getBindingDescriptions()
	{
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
		bindingDescriptions[0].binding = 0;
		bindingDescriptions[0].stride = sizeof(Vertex);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescriptions;
	}

	std::vector<VkVertexInputAttributeDescription> CgeModel::Vertex::getAttributeDescriptions()
	{
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, position);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex,color);
		return attributeDescriptions; 
	}

}