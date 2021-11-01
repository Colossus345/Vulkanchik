#include "CgeModel.h"


#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include"Utils.h"
#define GLM_ENABLE_EXPERIMENTAL
#include<glm/gtx/hash.hpp>


#include<iostream>
#include<unordered_map>

namespace std {
	template<>
	struct hash<cge::CgeModel::Vertex> {

		size_t operator()(cge::CgeModel::Vertex const& vertex) const {
			size_t seed = 0;
			cge::hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
			return seed;
		}
	};
}

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

	std::unique_ptr<CgeModel> CgeModel::createModelFromFile(CgeDevice& device, const std::string& filepath)
	{
		Data data{};
		data.loadModel(filepath);
		std::cout << "Vertex count-" << data.vertices.size() << "\n";
		return std::make_unique<CgeModel>(device, data);
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
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

		attributeDescriptions.push_back({ 0,0,VK_FORMAT_R32G32B32_SFLOAT , offsetof(Vertex, position) });
		attributeDescriptions.push_back({ 1,0,VK_FORMAT_R32G32B32_SFLOAT , offsetof(Vertex, color) });
		attributeDescriptions.push_back({ 2,0,VK_FORMAT_R32G32B32_SFLOAT , offsetof(Vertex, normal	) });
		attributeDescriptions.push_back({ 3,0,VK_FORMAT_R32G32_SFLOAT , offsetof(Vertex, uv) });

		
		return attributeDescriptions; 
	}

	void CgeModel::Data::loadModel(const std::string& filename)
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn, err;
		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str())) {
			throw std::runtime_error(warn + err);
		}

		vertices.clear();
		indices.clear();

		std::unordered_map<Vertex, uint32_t> uniqueVertices{};
		for (const auto& shape : shapes) {
			for (const auto& index : shape.mesh.indices) {
				Vertex vertex{};
				
				if (index.vertex_index >= 0) {
					vertex.position = {
						attrib.vertices[3 * index.vertex_index + 0],
						attrib.vertices[3 * index.vertex_index + 1],
						attrib.vertices[3 * index.vertex_index + 2],

					};
					vertex.color = {
						attrib.colors[3 * index.vertex_index + 0],
						attrib.colors[3 * index.vertex_index + 1],
						attrib.colors[3 * index.vertex_index + 2],

					};
					 
				}
				if (index.normal_index  >= 0) {
					vertex.normal = {
						attrib.normals[3 * index.normal_index + 0],
						attrib.normals[3 * index.normal_index + 1],
						attrib.normals[3 * index.normal_index + 2],

					};
				}
				if (index.texcoord_index  >= 0) {
					vertex.uv = {
						attrib.texcoords[2 * index.texcoord_index + 0],
						attrib.texcoords[2 * index.texcoord_index + 1],

					};
				}


				if (uniqueVertices.count(vertex) == 0) {
					uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
					vertices.push_back(vertex);
				}
				indices.push_back(uniqueVertices[vertex]);
			}
		}
	}



}