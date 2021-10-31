#include "CgeModel.h"

namespace cge {
	CgeModel::CgeModel(CgeDevice& device, const CgeModel::Data& data) :cgeDevice{ device } {
		createVertexBuffers(data.vertices);
		createIndexBuffers(data.indices);
	}

	CgeModel::~CgeModel() {
		vkDestroyBuffer(cgeDevice.device(), vertexBuffer, nullptr);
		vkFreeMemory(cgeDevice.device(), vertexBufferMemory, nullptr);
		if (hasIndexBuffer) {
			vkDestroyBuffer(cgeDevice.device(),	indexBuffer, nullptr);
			vkFreeMemory(cgeDevice.device(), indexBufferMemory, nullptr);
		}

	}

	void CgeModel::bind(VkCommandBuffer commandBuffer)
	{
		VkBuffer buffers[] = { vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
		if (hasIndexBuffer) {
			vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
		}
	}

	void CgeModel::draw(VkCommandBuffer commandBuffer)
	{
		if (hasIndexBuffer) {
			vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
		}
		else {


			vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
		}
	}

	void CgeModel::createVertexBuffers(const std::vector<Vertex>& verticies)
	{
		vertexCount = static_cast<uint32_t>(verticies.size());
		assert(vertexCount >= 3 && "vertex must be at least 3");
		VkDeviceSize bufferSize = sizeof(verticies[0]) * vertexCount;


		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		cgeDevice.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer,
			stagingBufferMemory
		);

		void* data;
		vkMapMemory(cgeDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, verticies.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(cgeDevice.device(), stagingBufferMemory);

		cgeDevice.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			vertexBuffer,
			vertexBufferMemory
		);
		cgeDevice.copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

		vkDestroyBuffer(cgeDevice.device(), stagingBuffer, nullptr);
		vkFreeMemory(cgeDevice.device(), stagingBufferMemory, nullptr);



	}

	void CgeModel::createIndexBuffers(const std::vector<uint32_t>& indicies)
	{
		indexCount = static_cast<uint32_t>(indicies.size());
		hasIndexBuffer = indexCount>0;
		
		if (!hasIndexBuffer) {
			return;
		}
		VkDeviceSize bufferSize = sizeof(indicies[0]) * indexCount;
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		cgeDevice.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer,
			stagingBufferMemory
		);

		void* data;
		vkMapMemory(cgeDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, indicies.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(cgeDevice.device(), stagingBufferMemory);

		cgeDevice.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			indexBuffer,
			indexBufferMemory
		);
		cgeDevice.copyBuffer(stagingBuffer, indexBuffer, bufferSize);

		vkDestroyBuffer(cgeDevice.device(), stagingBuffer, nullptr);
		vkFreeMemory(cgeDevice.device(), stagingBufferMemory, nullptr);
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
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, position);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex,color);
		return attributeDescriptions; 
	}

}