#pragma once

#include "Bobo/Window/Window.h"
#include "Bobo/ECS/Scenes/SceneManager.h"
#include "Bobo/Input/InputManager.h"

#include <GLFW/glfw3.h>

namespace Bobo
{
	class GameWindow : public Window
	{
	public:
		GameWindow(const WindowProps& props);
		virtual ~GameWindow();
		void Update() override;
		void EventLoop() override;
		inline unsigned int GetWidth() const override { return m_Props.width; }
		inline unsigned int GetHeight() const override { return m_Props.height; }
	private:
		GLFWwindow* p_Window;
		WindowProps m_Props;
	};
}