#pragma once

#include "../bpch.h"
#include <GLFW/glfw3.h>

// Key Input Class
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

	// Called whenever a key is pressed
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

	// get if the current key is being held down right now
	static bool GetKey(int key)
	{
		return p_KeyHoldArray[key];
	}

	// get if the current key has just been pressed
	static bool GetKeyDown(int key)
	{
		bool keyVal = p_KeyPressArray[key];
		p_KeyPressArray[key] = false;
		return keyVal;
	}

	// get if the current key has just been released
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