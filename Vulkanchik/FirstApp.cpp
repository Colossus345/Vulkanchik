#pragma once
#include "FirstApp.h"
#include "KeyBoardMovementController.h"
#include "CgeCamera.h"
#include "Simple_render_System.h"
#include "Cge_buffer.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE



#include<glm/glm.hpp>
#include<glm/gtc/constants.hpp>

#include<stdexcept>
#include<array>
#include<chrono>
#include<cassert>



namespace cge {

	struct GlobalUbo {
		glm::mat4 projectionView{ 1.f };
		glm::vec3 lightDirection = glm::normalize(glm::vec3(1.f, -3.f, -1.f));
	};


	FirstApp::FirstApp()
	{
		loadGameObjects();

	}

	FirstApp::~FirstApp()
	{
	}


	void FirstApp::run() {

		std::vector < std::unique_ptr<CgeBuffer> > uboBuffers(CgeSwapChain::MAX_FRAMES_IN_FLIGHT);

		for (int i = 0; i < uboBuffers.size(); i++) {
			uboBuffers[i] = std::make_unique<CgeBuffer>(cgeDevice,
				sizeof(GlobalUbo),
				1,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

			uboBuffers[i]->map();
		}



		SimpleRenderSystem simpleRenderSystem{ cgeDevice,cgeRenderer.getSwapChainRenderPass() };
        CgeCamera camera{};
        camera.setViewTarget(glm::vec3(-2.f, -1.f, 2.f), glm::vec3(0.f, 0.f, 2.5f));

        auto viewerObject = CgeGameObject::createGameObject();
        KeyBoardMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();

		while (!cgeWindow.shouldClose()) {
			glfwPollEvents();
            auto newTime = std::chrono::high_resolution_clock::now();

            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            cameraController.moveInPlaneXZ(cgeWindow.getGLFWwindow(), frameTime, viewerObject);

            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            
            float aspect= cgeRenderer.getAspectRation();

          
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);
			if (auto commandBuffer = cgeRenderer.beginFrame()) {
				int frameIndex = cgeRenderer.getFrameIndex();
				FrameInfo frameInfo{
					frameIndex,
					frameTime,
					commandBuffer,
					camera
				};
				GlobalUbo ubo{};
				ubo.projectionView = camera.getProjection() * camera.getView();
				uboBuffers[frameIndex]->writeToBuffer(&ubo);
				uboBuffers[frameIndex]->flush();

				//render
				cgeRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(frameInfo, gameObjects);
				cgeRenderer.endSwapChainRenderPass(commandBuffer);
				cgeRenderer.endFrame();
			}
		}
		vkDeviceWaitIdle(cgeDevice.device());
	}

	void FirstApp::loadGameObjects()
	{
        std::shared_ptr<CgeModel> cgeModel = CgeModel::createModelFromFile(cgeDevice,"C:\\Users\\Syndafloden\\source\\repos\\Vulkanchik\\models\\flat_vase.obj");

        auto flat = CgeGameObject::createGameObject();
		flat.model = cgeModel;
		flat.transform.translation = {-.5f,.5f,2.5f };
		flat.transform.scale = { 3.f,1.5f,3.f };
        gameObjects.push_back(std::move(flat));

		cgeModel = CgeModel::createModelFromFile(cgeDevice, "C:\\Users\\Syndafloden\\source\\repos\\Vulkanchik\\models\\smooth_vase.obj");

		auto smooth = CgeGameObject::createGameObject();
		smooth.model = cgeModel;
		smooth.transform.translation = { .5f,.5f,2.5f };
		smooth.transform.scale = { 3.f,1.5f,3.f };
		gameObjects.push_back(std::move(smooth));
	}
}