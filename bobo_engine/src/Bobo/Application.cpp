#include "bobopch.h"
#include "Application.h"
#include "ECS/Scene.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Bobo
{
	Application::Application()
	{
		Bobo::Log::Init();
		BOBO_TRACE("Logger initialized...");
		p_Window = std::unique_ptr<Window>(Window::Create());
		glm::vec4 position = glm::vec4(0, 0, 0, 1);

		unsigned int test;
		glGenVertexArrays(1, &test);

		// Testing Assert
		BOBO_ASSERT(true, "This should print false");

		// Testing ECS
		Scene* s = new Scene();

		auto entity = s->CreateEntity();
		
		BOBO_ASSERT(s->AddComponent<Transform>(entity, glm::vec3(43, 1, 1), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1)), "Add component failed.");

		auto entityTransform = s->GetComponent<Transform>(entity);

		BOBO_INFO("x: {}", entityTransform->position.x);

		auto child = s->CreateEntity(entity);

		s->DestroyEntity(entity);

		BOBO_INFO("ECS test complete");

		// Testing Systems
		s->InitialiseMandatorySystems();

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		p_Window->EventLoop();
	}
}