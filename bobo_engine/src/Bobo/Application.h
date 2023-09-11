#pragma once

#include "Core.h"

namespace Bobo
{
	class BOBO_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	Application* CreateApplication();
}
