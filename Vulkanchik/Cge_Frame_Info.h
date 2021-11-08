#pragma once


#include "CgeCamera.h"

#include <vulkan/vulkan.h>

namespace cge {
	struct FrameInfo {
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		CgeCamera& camera;
	};

}