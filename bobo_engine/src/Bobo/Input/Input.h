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
				s_KeyPressArray[i] = false;
				s_KeyHoldArray[i] = false;
				s_KeyReleaseArray[i] = false;
			}

			glfwSetKeyCallback(window, KeyCallback);
		}

		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (action == GLFW_PRESS)
			{
				s_KeyPressArray[key] = true;
				s_KeyHoldArray[key] = true;
			}
			else if (action == GLFW_RELEASE)
			{
				s_KeyReleaseArray[key] = true;
				s_KeyHoldArray[key] = false;
			}
		}

		static bool GetKey(const int key)
		{
			return s_KeyHoldArray[key];
		};
		
		static bool GetKeyDown(const int key)
		{
			bool keyVal = s_KeyPressArray[key];
			s_KeyPressArray[key] = false;
			return keyVal;
		};

		static bool GetKeyUp(const int key)
		{
			bool keyVal = s_KeyReleaseArray[key];
			s_KeyReleaseArray[key] = false;
			return keyVal;
		};

	private:
		bool static s_KeyPressArray[GLFW_KEY_LAST];
		bool static s_KeyHoldArray[GLFW_KEY_LAST];
		bool static s_KeyReleaseArray[GLFW_KEY_LAST];
	};
}