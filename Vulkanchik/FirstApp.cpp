#pragma once
#include "FirstApp.h"
#include "KeyBoardMovementController.h"
#include "CgeCamera.h"
#include "Simple_render_System.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE



#include<glm/glm.hpp>
#include<glm/gtc/constants.hpp>

#include<stdexcept>
#include<array>
#include<chrono>
#include<cassert>



namespace cge {



	FirstApp::FirstApp()
	{
		loadGameObjects();

	}

	FirstApp::~FirstApp()
	{
	}


	void FirstApp::run() {
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

          
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);
			if (auto commandBuffer = cgeRenderer.beginFrame()) {
				cgeRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
				cgeRenderer.endSwapChainRenderPass(commandBuffer);
				cgeRenderer.endFrame();
			}
		}
		vkDeviceWaitIdle(cgeDevice.device());
	}

	void FirstApp::loadGameObjects()
	{
        std::shared_ptr<CgeModel> cgeModel = CgeModel::createModelFromFile(cgeDevice,"C:\\Users\\Syndafloden\\source\\repos\\Vulkanchik\\models\\flat_vase.obj");

        auto cube = CgeGameObject::createGameObject();
        cube.model = cgeModel;
        cube.transform.translation = { .0f,.0f,2.5f };
        cube.transform.scale = { .5f,.5f,.5f };
        gameObjects.push_back(std::move(cube));
	}
}