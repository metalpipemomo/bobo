#pragma once

#include "bpch.h"
#include <GLFW/glfw3.h>

class Input
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

	static bool GetKey(int key)
	{
		return p_KeyHoldArray[key];
	}

	static bool GetKeyDown(int key)
	{
		bool keyVal = p_KeyPressArray[key];
		p_KeyPressArray[key] = false;
		return keyVal;
	}

	static bool GetKeyUp(int key)
	{
		bool keyVal = p_KeyReleaseArray[key];
		p_KeyReleaseArray[key] = false;
		return keyVal;
	}

private:
	static inline bool p_KeyPressArray[GLFW_KEY_LAST];
	static inline bool p_KeyHoldArray[GLFW_KEY_LAST];
	static inline bool p_KeyReleaseArray[GLFW_KEY_LAST];
};