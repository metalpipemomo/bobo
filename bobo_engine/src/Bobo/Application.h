#pragma once

#include "Core.h"
#include "Window/Window.h"

namespace Bobo
{
	class BOBO_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
	private:
		std::unique_ptr<Window> p_Window;
	};

	Application* CreateApplication();
}
