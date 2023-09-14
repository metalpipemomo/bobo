#include "bobopch.h"
#include "Application.h"
#include "ECS/Entity.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Bobo
{
	class TestComponent : public Component
	{
	public:
		void LogSomething() { BOBO_CORE_ERROR("BRUHHHHH"); }
	};

	Application::Application()
	{
		Bobo::Log::Init();
		BOBO_CORE_TRACE("Logger initialized...");
		p_Window = std::unique_ptr<Window>(Window::Create());
		glm::vec4 position = glm::vec4(0, 0, 0, 1);

		unsigned int test;
		glGenVertexArrays(1, &test);

		BOBO_ASSERT(true, "This should print false");

		Entity* e = new Entity();
		e->AddComponent(new TestComponent());
		BOBO_ASSERT(e->HasComponent<TestComponent>(), "Component didn't exist apparently.");
		auto tc = e->GetComponent<TestComponent>();
		tc->LogSomething();
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		p_Window->EventLoop();
	}
}