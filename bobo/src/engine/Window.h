#pragma once

#include "bpch.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Input.h"
#include "Model/ModelLoader.h"
#include "Audio/Audio.h"
#include "Time.h"
#include "Coroutine/CoroutineScheduler.h"

struct WindowProperties
{
	unsigned int width, height;
	std::string name;

	WindowProperties(
		unsigned int width = 640,
		unsigned int height = 480, 
		const std::string& name = "Default Window"
	)
		: width(width), height(height), name(name) {}
};

class Window
{
public:
	Window(const WindowProperties& props = WindowProperties{})
	{
		m_Props = props;

		// Init stuff here, order matters
		Log::Init();
		Init();
		ModelLoader::Init();
		Audio::Init();
		Input::Init(p_Window);
		Time::Init();
		CoroutineScheduler::Init();
	}

	~Window()
	{
		glfwDestroyWindow(p_Window);
	}

	void EventLoop()
	{
		while (!glfwWindowShouldClose(p_Window))
		{
			// System Frame Updates
			Time::Update();
			Audio::Update();
			CoroutineScheduler::Update();

			// System Fixed Updates
			if (Time::DidFixedUpdate())
			{

			}

			// Clear Screen
			glClearColor(0, 1, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Draw

			// Window Updates
			Update();

			// Funny exit thing, also input test
			if (Input::GetKey(GLFW_KEY_L) &&
				Input::GetKey(GLFW_KEY_M) &&
				Input::GetKey(GLFW_KEY_A) &&
				Input::GetKey(GLFW_KEY_O))
			{
				exit(-1);
			}
		}
	}

	inline unsigned int GetWidth() const
	{
		return m_Props.width;
	}

	inline unsigned int GetHeight() const
	{
		return m_Props.height;
	}

private:
	void Init()
	{
		if (!glfwInit())
		{
			BOBO_ERROR("Failed to initialize GLFW...");
			return;
		}
		BOBO_INFO("GLFW initialized!");

		p_Window = glfwCreateWindow(m_Props.width, m_Props.height, m_Props.name.c_str(), NULL, NULL);

		if (!p_Window)
		{
			BOBO_ERROR("Failed to create window...");
			glfwTerminate();
			return;
		}
		BOBO_INFO("Window created!");

		glfwMakeContextCurrent(p_Window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			BOBO_ERROR("Failed to initialize Glad...");
			return;
		}
		BOBO_INFO("Glad initialized!");
	}

	void Update()
	{
		glfwPollEvents();
		glfwSwapBuffers(p_Window);
	}

	GLFWwindow* p_Window;
	WindowProperties m_Props;
};