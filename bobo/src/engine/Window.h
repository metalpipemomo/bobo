#pragma once

#include "bpch.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Input.h"
#include "Model/ModelLoader.h"
#include "Audio/Audio.h"
#include "Time.h"
#include "EntityComponent/SceneManager.h"
#include "Coroutine/CoroutineScheduler.h"
#include "Renderer/Camera.h"
#include "Renderer/TextureLoader.h"
#include "Renderer/ShaderLoader.h"
#include "Renderer/Renderer.h"

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
		InitImGui();
		Input::Init(p_Window);
		Time::Init();
		Camera::Init((float)GetWidth() / (float)GetHeight());
		TextureLoader::Init();
		ShaderLoader::Init();
		Renderer::Init();
		
		SceneManager::Init();
		SceneManager::AddOnSceneChanged("SetSceneHasChanged", [this]() { hasSceneChanged = true;});
		
		ModelLoader::Init();
		Audio::Init();
		CoroutineScheduler::Init();
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
			if (hasSceneChanged)
			{
				SceneManager::AwakeActiveScene();
				hasSceneChanged = false;
			}

			// ImGui Frame Updates
			CreateImGuiForGame();

			// System Frame Updates
			Renderer::Update();
			Time::Update();
			Audio::Update();
			CoroutineScheduler::Update();
			SceneManager::UpdateActiveScene();

			// System Fixed Updates
			if (Time::DidFixedUpdate())
			{
				SceneManager::FixedUpdateActiveScene();
			}

			// Clear Screen
			glClearColor(0, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Draw
			Renderer::Draw();

			// ImGui Render call
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

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
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
	}

	void InitImGui()
	{
		// Create ImGui Context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		BOBO_INFO("ImGui initialized");

		// Optionally set configuration flags, load fonts, setup style
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

		// Initialize Platform and Rendering backends
		ImGui_ImplGlfw_InitForOpenGL(p_Window, true);
		ImGui_ImplOpenGL3_Init();
	}

	void CreateImGuiForGame() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Set the various window flags.
		static bool no_titlebar = false;
		static bool no_scrollbar = true;
		static bool no_menu = true;
		static bool no_move = true;
		static bool no_resize = true;
		static bool no_collapse = true;
		static bool no_close = true;
		static bool no_nav = true;
		static bool no_background = false;
		static bool no_bring_to_front = false;
		static bool unsaved_document = false;

		ImGuiWindowFlags window_flags = 0;
		if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
		if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
		if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
		if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
		if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
		if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
		if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
		if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
		if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
		if (unsaved_document)   window_flags |= ImGuiWindowFlags_UnsavedDocument;

		// Set the window size.
		const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 100, main_viewport->WorkPos.y + 20), 0);
		ImGui::SetNextWindowSize(ImVec2(440, 70), 0);

		ImGui::Begin("Score", NULL, window_flags);
		ImGui::Text("Solid Balls Remaining: 7");
		ImGui::Text("Striped Balls Remaining: 7");
		ImGui::End();
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