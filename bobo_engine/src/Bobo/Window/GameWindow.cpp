#include "bobopch.h"
#include "GameWindow.h"

#include <glad/glad.h>

//johnnystuff remove
#include "../PhysicsWorld.h"
#include <chrono>
#include <thread>
///
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
		
		//johnnystuff remove
		//PhysicsWorld& p = PhysicsWorld::GetInstance();
		//GameObject test;
		//GameObject test2;
		//SphereCollider col1;
		//col1.Center = Vector3();
		//test.transform = new Transform();
		//test.Collider = &col1;
		//test.mass = 1;
		//test.velocity = Vector3();
		//test.force = Vector3();
		//p.AddObject(&test);

		//SphereCollider col2;
		//col2.Center = Vector3();
		//col2.Radius = 1;
		//test2.transform = new Transform();
		//test2.Collider = &col2;
		//test2.mass = 100;
		//test2.velocity = Vector3();
		//test2.force = Vector3();
		//test2.hasGravity = false;
		//p.AddObject(&test2);
		//std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		//p.CalculateDeltaTime();
		/////
		while (!glfwWindowShouldClose(p_Window))
		{
			//float deltaTime = p.CalculateDeltaTime();
			//BOBO_CORE_INFO("DeltaTime: " + std::to_string(deltaTime));
			glClearColor(0, 1, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			Update();

			//johnnystuff remove
			//p.Update(deltaTime);
			//
		}

	}
}