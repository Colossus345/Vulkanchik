#pragma once

#define GLFW_INCLUDE_VULKAN
#include <string>
#include<GLFW/glfw3.h>
namespace cge
{
	class CgeWindow {
	public:
		CgeWindow(int w, int h, std::string Name);
		~CgeWindow();

		CgeWindow(const CgeWindow&) = delete;
		CgeWindow& operator =(const CgeWindow&) = delete;

	bool shouldClose(){ return glfwWindowShouldClose(window); }

	VkExtent2D getExtend() { return { static_cast<uint32_t>(width),static_cast<uint32_t>(height) }; }
	
	bool wasWindowResized() { return framebufferResized; }
	void resetWindowResized() { framebufferResized = false; }

	void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);


	private:
		static void framebufferResizeCallback(GLFWwindow *window, int width, int height);

		void initWindow();
		int width;
		int height;
		bool framebufferResized = false;	

		std::string windowName;
		GLFWwindow* window;
	};

};

