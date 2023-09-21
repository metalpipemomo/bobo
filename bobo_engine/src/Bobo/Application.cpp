#include "bobopch.h"
#include "Application.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Bobo
{
	Application::Application()
	{
		Bobo::Log::Init();
		BOBO_TRACE("Logger initialized...");
		p_Window = std::unique_ptr<Window>(Window::Create());
		glm::vec4 position = glm::vec4(0, 0, 0, 1);

		unsigned int test;
		glGenVertexArrays(1, &test);

		// Testing Assert
		BOBO_ASSERT(true, "This should print false");
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		p_Window->EventLoop();
	}
}