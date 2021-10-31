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

    std::unique_ptr<CgeModel> createCubeModel(CgeDevice& device, glm::vec3 offset) {
        CgeModel::Data modelBuilder{};
        modelBuilder.vertices = {
            // left face (white)
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},

            // right face (yellow)
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},

            // top face (orange, remember y axis points down)
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},

            // bottom face (red)
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},

            // nose face (blue)
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},

            // tail face (green)
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        };
        for (auto& v : modelBuilder.vertices) {
            v.position += offset;
        }

        modelBuilder.indices = { 0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9,
                                12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21 };

        return std::make_unique<CgeModel>(device, modelBuilder);
    }
	void FirstApp::loadGameObjects()
	{
        std::shared_ptr<CgeModel> cgeModel = createCubeModel(cgeDevice, { 0.f,0.f,0.f });

        auto cube = CgeGameObject::createGameObject();
        cube.model = cgeModel;
        cube.transform.translation = { .0f,.0f,2.5f };
        cube.transform.scale = { .5f,.5f,.5f };
        gameObjects.push_back(std::move(cube));
	}
}