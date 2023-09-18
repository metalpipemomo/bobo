#include "bobopch.h"
#include "GameWindow.h"

#include <glad/glad.h>

namespace Bobo
{
	Window* Window::Create(const WindowProps& props)
	{
		return new GameWindow(props);
	}

	GameWindow::GameWindow(const WindowProps& props)
	{
		m_Props = props;

		if (!glfwInit())
		{
			BOBO_CORE_ERROR("Could not initialize GLFW.");
			return;
		}
		BOBO_CORE_TRACE("GLFW initialized...");

		p_Window = glfwCreateWindow(m_Props.width, m_Props.height, m_Props.title.c_str(), NULL, NULL);

		if (!p_Window)
		{
			BOBO_CORE_ERROR("Could not create window.");
			glfwTerminate();
			return;
		}
		BOBO_CORE_TRACE("Window successfully created...");

		glfwMakeContextCurrent(p_Window);
		
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			BOBO_CORE_ERROR("Glad failed to initialize...");
			return;
		}
		BOBO_CORE_TRACE("Glad initialized...");
	}

	GameWindow::~GameWindow()
	{
		glfwDestroyWindow(p_Window);
	}

	void GameWindow::Update()
	{
		glfwPollEvents();
		glfwSwapBuffers(p_Window);
	}

	void GameWindow::EventLoop()
	{
		while (!glfwWindowShouldClose(p_Window))
		{
			glClearColor(0, 1, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			Update();
		}
	}
}