#include "bobopch.h"
#include "GameWindow.h"

#include <glad/glad.h>
#include <Bobo/Time.h>

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
		CoroutineScheduler cs = CoroutineScheduler::GetInstance();
		Time c_Time = Time::GetInstance();

		c_Time.StartCallAfterTime(CallbackContainer ([&]() { c_Time.SetTimeScale(5); }), 3);

		while (!glfwWindowShouldClose(p_Window))
		{
			c_Time.Update();

			// Update Scene
			if (c_Time.DidFixedUpdate()) c_SceneManager.FixedUpdateLoadedScene();
			c_SceneManager.UpdateLoadedScene();
			cs.Update();

			glClearColor(0, 1, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			Update();
		}
	}
}