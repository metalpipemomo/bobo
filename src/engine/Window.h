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
#include "Popups/PopupManager.h"

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
			Camera::Update();

			// Update Audio & 3d Listener
			glm::vec3 camPos = Camera::GetPosition();
			glm::vec3 camFront = Camera::GetCameraFront();
			glm::vec3 camUp = Camera::GetCameraUp();
			Audio::Set3DListenerPosition(
				camPos.x, camPos.y, camPos.z,
				camFront.x, camFront.y, camFront.z,
				camUp.x, camUp.y, camUp.z
			);
			Audio::Update();

			CoroutineScheduler::Update();
			SceneManager::UpdateActiveScene();
			NotificationManager::Update();
			PopupManager::Update();

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
			Camera::HandleCameraEvents(); // Abstracted to the Camera Class.
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