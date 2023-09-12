#pragma once

#include "bobopch.h"

#include "Bobo/Core.h"

namespace Bobo
{
	struct WindowProps
	{
		unsigned int width;
		unsigned int height;
		std::string title;
		WindowProps(int width = 640, int height = 480, const std::string& title = "Bobo throws ballz")
			: width(width), height(height), title(title) {}
	};

	class BOBO_API Window
	{
	public:
		static Window* Create(const WindowProps& props = WindowProps());
		virtual ~Window() {}
		virtual void Update() = 0;
		virtual void EventLoop() = 0;
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
	};
}