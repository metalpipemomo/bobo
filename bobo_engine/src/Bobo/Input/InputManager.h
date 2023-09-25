#pragma once

#include "bobopch.h"
#include <GLFW/glfw3.h>

namespace Bobo
{
	class InputManager
	{
	public:
		static InputManager& GetInstance()
		{
			static InputManager* instance = new InputManager();
			return *instance;
		}
		virtual ~InputManager();
		void KeyCallback(int key, int action);
		void MouseCallback(int button, int action);

	private:
		GLFWwindow* p_currentWindow;
		InputManager();
	};
}