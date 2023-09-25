#pragma once

#include "bobopch.h"
#include <GLFW/glfw3.h>

namespace Bobo
{
	class InputManager
	{
	public:
		static void InputInit(GLFWwindow* window)
		{
			for (int i = 0; i < sizeof(p_KeyPressArray); i++) {
				p_KeyPressArray[i] = false;
			}

			for (int i = 0; i < sizeof(p_KeyHoldArray); i++) {
				p_KeyHoldArray[i] = false;
			}

			for (int i = 0; i < sizeof(p_KeyReleaseArray); i++) {
				p_KeyReleaseArray[i] = false;
			}

			glfwSetKeyCallback(window, KeyCallback);
		}

		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (action == GLFW_PRESS) {
				p_KeyPressArray[key] = true;
				p_KeyHoldArray[key] = true;
			}
			else if (action == GLFW_RELEASE) {
				p_KeyReleaseArray[key] = true;
				p_KeyHoldArray[key] = false;
			}
		}

		static bool GetKeyPress(const int key)
		{
			bool keyVal = p_KeyPressArray[key];
			p_KeyPressArray[key] = false;
			return keyVal;
		};

		static bool GetKey(const int key)
		{
			return p_KeyHoldArray[key];
		};

		static bool GetKeyUp(const int key)
		{
			bool keyVal = p_KeyReleaseArray[key];
			p_KeyReleaseArray[key] = false;
			return keyVal;
		};

	private:
		bool static p_KeyPressArray[GLFW_KEY_LAST];
		bool static p_KeyHoldArray[GLFW_KEY_LAST];
		bool static p_KeyReleaseArray[GLFW_KEY_LAST];
	};

	bool InputManager::p_KeyPressArray[GLFW_KEY_LAST];
	bool InputManager::p_KeyHoldArray[GLFW_KEY_LAST];
	bool InputManager::p_KeyReleaseArray[GLFW_KEY_LAST];
}