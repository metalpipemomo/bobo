#include "bobopch.h"
#include "InputManager.h"

namespace Bobo
{
	InputManager::InputManager()
	{
		
	}
	InputManager::~InputManager()
	{

	}

	void InputManager::KeyCallback(int key, int action)
	{
		if (key == GLFW_KEY_A && action == GLFW_PRESS)
		{
			// move pool cue to the left
		}
		else if (key == GLFW_KEY_D && action == GLFW_PRESS)
		{
			// move pool cue to the right
		}
		// don't know if we are doing the other directions 
	}

	void InputManager::MouseCallback(int button, int action)
	{
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		{
			// shoot pool cue
		}
	}


}