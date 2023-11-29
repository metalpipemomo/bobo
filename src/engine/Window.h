#pragma once

#include "../bpch.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Physics/Physics.h"

#include "Input.h"
#include "Model/ModelLoader.h"
#include "Audio/Audio.h"
#include "Time.h"
#include "EntityComponent/SceneManager.h"
#include "GameState/GameStateManager.h"
#include "Coroutine/CoroutineScheduler.h"
#include "Renderer/Camera.h"
#include "Renderer/TextureLoader.h"
#include "Renderer/Renderer.h"
#include "Notifications/NotificationManager.h"

struct WindowProperties
{
	unsigned int width, height;
	std::string name;

	WindowProperties(
		unsigned int width,
		unsigned int height,
		const std::string& name
	)
		: width(width), height(height), name(name) {}
};

class Window
{
public:
	Window() : m_Props(WindowProperties(WINDOW_WIDTH, WINDOW_HEIGHT, "Totally Accurate Pool Simulator"))
	{
		// Init stuff here, order matters
		Log::Init();
		Init();
		InitImGui();
		Input::Init(p_Window);
		Time::Init();
		Camera::Init((float)GetWidth() / (float)GetHeight());
		TextureLoader::Init();
		ModelLoader::Init();
		Renderer::Init();
		
		SceneManager::Init();
		SceneManager::AddOnSceneChanged("SetSceneHasChanged", [this]() { hasSceneChanged = true;});
		hasSceneChanged = true;
		
		Audio::Init();
		CoroutineScheduler::Init();

		Physics::Init();
	}

	~Window()
	{
		// Shutdown ImGui
		ImGui_ImplGlfw_Shutdown();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow(p_Window);
	}

	void EventLoop()
	{
		while (!glfwWindowShouldClose(p_Window))
		{
			// ImGUI Frame Update
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			if (hasSceneChanged)
			{
				SceneManager::AwakeActiveScene();
				hasSceneChanged = false;
			}

			// System Frame Updates
			Renderer::Update();
			Time::Update();
			Audio::Update();
			CoroutineScheduler::Update();
			SceneManager::UpdateActiveScene();
			NotificationManager::Update();

			// Pause on pressing Escape
			if (Input::GetKeyDown(GLFW_KEY_ESCAPE))
			{
				GameStateManager::TogglePauseState();
			}
			GameStateManager::UpdateCurrentGameState();

			// System Fixed Updates
			if (Time::DidFixedUpdate())
			{
				SceneManager::FixedUpdateActiveScene();
				Physics::Update();
			}

			// Clear Screen
			glClearColor(0, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Draw
			Renderer::Draw();

			// ImGui Render call
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			if (Input::GetKey(GLFW_KEY_W) && Input::GetKey(GLFW_KEY_LEFT_CONTROL))
			{
				Camera::MoveUp(10.0f * Time::DeltaTime());
			} else if (Input::GetKey(GLFW_KEY_W))
			{
				Camera::MoveForward(10.0f * Time::DeltaTime());
			}
			if (Input::GetKey(GLFW_KEY_S) && Input::GetKey(GLFW_KEY_LEFT_CONTROL))
			{
				Camera::MoveUp(-10.0f * Time::DeltaTime());
			} else if (Input::GetKey(GLFW_KEY_S))
			{
				Camera::MoveForward(-10.0f * Time::DeltaTime());
			}
			
			if (Input::GetKey(GLFW_KEY_D))
			{
				Camera::MoveRight(10.0f * Time::DeltaTime());
			}
			if (Input::GetKey(GLFW_KEY_A))
			{
				Camera::MoveRight(-10.0f * Time::DeltaTime());
			}

			if(Input::GetKey(GLFW_KEY_UP)) {
			
				Camera::LookUp(50.0f * Time::DeltaTime());
			}

			if(Input::GetKey(GLFW_KEY_DOWN)) {
			
				Camera::LookUp(-50.0f * Time::DeltaTime());
			}

			if(Input::GetKey(GLFW_KEY_LEFT)) {
			
				Camera::LookRight(-50.0f * Time::DeltaTime());
			}

			if(Input::GetKey(GLFW_KEY_RIGHT)) {
			
				Camera::LookRight(50.0f * Time::DeltaTime());
			}

			if (Input::GetKeyDown(GLFW_KEY_X))
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else if (Input::GetKeyDown(GLFW_KEY_Y))
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

			// Window Updates
			Update();
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

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_SAMPLES, 16);

		p_Window = glfwCreateWindow(m_Props.width, m_Props.height, m_Props.name.c_str(), NULL, NULL);

		if (!p_Window)
		{
			BOBO_ERROR("Failed to create window...");
			glfwTerminate();
			return;
		}
		BOBO_INFO("Window initialized!");

		glfwMakeContextCurrent(p_Window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			BOBO_ERROR("Failed to initialize Glad...");
			return;
		}
		BOBO_INFO("Glad initialized!");

		glEnable(GL_DEPTH_TEST);
    	glEnable(GL_MULTISAMPLE);
    	glEnable(GL_BLEND);
    	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    	glEnable(GL_CULL_FACE);
    	glCullFace(GL_BACK);
	}

	void InitImGui()
	{
		// Create ImGui Context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		// Optionally set configuration flags, load fonts, setup style
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

		// Initialize Platform and Rendering backends
		ImGui_ImplGlfw_InitForOpenGL(p_Window, true);
		ImGui_ImplOpenGL3_Init();

		BOBO_INFO("ImGui initialized!");
	}

	void Update()
	{
		glfwPollEvents();
		glfwSwapBuffers(p_Window);
	}

	GLFWwindow* p_Window;
	WindowProperties m_Props;
	bool hasSceneChanged = false;
};