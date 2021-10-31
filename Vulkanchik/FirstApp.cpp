#include "FirstApp.h"
#include "Simple_render_System.h"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include<glm/glm.hpp>
#include<glm/gtc/constants.hpp>

#include<stdexcept>
#include<array>
#include<cassert>



namespace cge {


	void FirstApp::sierpienskie(std::vector<CgeModel::Vertex>& verticies, int depth, glm::vec2 left, glm::vec2 right, glm::vec2 top)
	{
		if (depth <= 0) {
			verticies.push_back({ {left} });
			verticies.push_back({ {right} });
			verticies.push_back({ {top} });
		}
		else {
			auto nleft = 0.5f * (left + top);
			auto ntop = 0.5f * (left + right);
			auto nright = 0.5f * (right + top);

			sierpienskie(verticies, depth - 1, left, ntop, nleft);
			sierpienskie(verticies, depth - 1, ntop, right, nright);
			sierpienskie(verticies, depth - 1, nleft, nright, top);
		}
	}
	FirstApp::FirstApp()
	{
		loadGameObjects();

	}

	FirstApp::~FirstApp()
	{
	}


	void FirstApp::run() {
		SimpleRenderSystem simpleRenderSystem{ cgeDevice,cgeRenderer.getSwapChainRenderPass() };

		while (!cgeWindow.shouldClose()) {
			glfwPollEvents();
			if (auto commandBuffer = cgeRenderer.beginFrame()) {
				cgeRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
				cgeRenderer.endSwapChainRenderPass(commandBuffer);
				cgeRenderer.endFrame();
			}
		}
		vkDeviceWaitIdle(cgeDevice.device());
	}
	void FirstApp::loadGameObjects()
	{
		std::vector<CgeModel::Vertex> verticies{
			{{ -0.5f,0.5f },{1.0f,0.0f,0.0f}},
			{{ 0.5f,0.5f },{0.0f,1.0f,0.0f}},
			{{ 0.0f,-0.5f },{0.0f,0.0f,1.0f}},
		};


		auto cgeModel = std::make_shared<CgeModel>(cgeDevice, verticies);

		auto triangle = CgeGameObject::createGameObject();
		triangle.model = cgeModel;
		triangle.color = { .1f,.8f,.1f };
		triangle.transform2d.translation.x = .2f;
		triangle.transform2d.scale = { 2.f,.5f };
		triangle.transform2d.rotation = .25f * glm::two_pi<float>();
		gameObjects.push_back(std::move(triangle));
	}
}