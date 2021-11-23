#pragma once


#include "CgeCamera.h"
#include "CgeGameObject.h"

#include <vulkan/vulkan.h>

namespace cge {
	struct FrameInfo {
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		CgeCamera& camera;
		VkDescriptorSet globalDescriptorSet;
		CgeGameObject::Map& gameObjects;
	};

}