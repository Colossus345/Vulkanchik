#include "CG_window.h"
#include <stdexcept>

namespace cge {
	CgeWindow::CgeWindow(int w, int h, std::string name) : width{ w }, height{ h }, windowName{ name }{
		initWindow();
	}
	CgeWindow::~CgeWindow() {
		glfwDestroyWindow(window);
		glfwTerminate(); 
	}
	
	void CgeWindow::initWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API,GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
		 
	}
	void CgeWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto cgeWindow = reinterpret_cast<CgeWindow *>(glfwGetWindowUserPointer(window));
		cgeWindow->framebufferResized = true;
		cgeWindow->width = width;
		cgeWindow->height = height;
	}
	void CgeWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface");
		}
	}
}