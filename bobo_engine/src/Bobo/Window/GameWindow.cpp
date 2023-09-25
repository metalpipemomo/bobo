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
			BOBO_ERROR("Could not initialize GLFW.");
			return;
		}
		BOBO_TRACE("GLFW initialized...");

		p_Window = glfwCreateWindow(m_Props.width, m_Props.height, m_Props.title.c_str(), NULL, NULL);

		if (!p_Window)
		{
			BOBO_ERROR("Could not create window.");
			glfwTerminate();
			return;
		}
		BOBO_TRACE("Window successfully created...");

		glfwMakeContextCurrent(p_Window);
		
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			BOBO_ERROR("Glad failed to initialize...");
			return;
		}
		BOBO_TRACE("Glad initialized...");
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
		SceneManager c_SceneManager = SceneManager::GetInstance();
		InputManager c_InputManager = InputManager::GetInstance();

		// For Fixed Update
		int fixedDeltaTime = 16; // this would be a static 60fps
		auto lastTime = std::chrono::high_resolution_clock::now();
		auto currentTime = std::chrono::high_resolution_clock::now();

		while (!glfwWindowShouldClose(p_Window))
		{
			currentTime = std::chrono::high_resolution_clock::now();
			auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime);

			if (deltaTime.count() >= fixedDeltaTime)
			{
				c_SceneManager.FixedUpdateLoadedScene();
				lastTime = currentTime;
			}

			c_SceneManager.UpdateLoadedScene();

			glClearColor(0, 1, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			Update();
		}

	}
}