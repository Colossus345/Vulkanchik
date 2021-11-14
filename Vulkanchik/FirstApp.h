#pragma once

#include"CG_window.h"
#include"CgeDevice.h"
#include"CgeGameObject.h"
#include"CgeRenderer.h"
#include"Cge_Descriptors.h"





#include<memory>
#include<vector>

namespace cge {
	class FirstApp {

	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void sierpienskie(std::vector<CgeModel::Vertex> &verticies,int depth, glm::vec2 left, glm::vec2 right, glm::vec2 top);
		FirstApp();
		~FirstApp();

		FirstApp(const FirstApp&) = delete;
		FirstApp& operator =(const FirstApp&) = delete;

		void run();

	private:

		void loadGameObjects();

		CgeWindow cgeWindow{ WIDTH,HEIGHT,"Hello Vulkan" };
		CgeDevice cgeDevice{ cgeWindow };
		CgeRenderer cgeRenderer{ cgeWindow,cgeDevice };

		std::unique_ptr<CgeDescriptorPool> globalPool{};
		std::vector<CgeGameObject> gameObjects;

		
	};
}