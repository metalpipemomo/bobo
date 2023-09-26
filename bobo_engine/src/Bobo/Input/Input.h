#pragma once

#include "bobopch.h"
#include <GLFW/glfw3.h>

namespace Bobo
{
	class BOBO_API Input
	{
	public:
		static void Init(GLFWwindow* window)
		{
			for (int i = 0; i < GLFW_KEY_LAST; i++)
			{
				p_KeyPressArray[i] = false;
				p_KeyHoldArray[i] = false;
				p_KeyReleaseArray[i] = false;
			}

			glfwSetKeyCallback(window, KeyCallback);
		}

		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (action == GLFW_PRESS)
			{
				p_KeyPressArray[key] = true;
				p_KeyHoldArray[key] = true;
			}
			else if (action == GLFW_RELEASE)
			{
				p_KeyReleaseArray[key] = true;
				p_KeyHoldArray[key] = false;
			}
		}

		static bool GetKey(const int key)
		{
			return p_KeyHoldArray[key];
		};
		
		static bool GetKeyDown(const int key)
		{
			bool keyVal = p_KeyPressArray[key];
			p_KeyPressArray[key] = false;
			return keyVal;
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
}