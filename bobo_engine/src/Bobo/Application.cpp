#include "bobopch.h"
#include "Application.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace Bobo
{
	Application::Application()
	{
		p_Window = std::unique_ptr<Window>(Window::Create());
		glm::vec4 position = glm::vec4(0, 0, 0, 1);
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		p_Window->EventLoop();
	}
}