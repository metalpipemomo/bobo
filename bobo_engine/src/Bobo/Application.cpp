#include "bobopch.h"
#include "Application.h"

#include <GLFW/glfw3.h>

namespace Bobo
{
	Application::Application()
	{
		p_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		p_Window->EventLoop();
	}
}