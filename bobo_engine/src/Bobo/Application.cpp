#include "bobopch.h"
#include "Application.h"
#include "ECS/Entity.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Bobo
{
	Application::Application()
	{
		Bobo::Log::Init();
		BOBO_CORE_TRACE("Logger initialized...");
		p_Window = std::unique_ptr<Window>(Window::Create());
		glm::vec4 position = glm::vec4(0, 0, 0, 1);

		unsigned int test;
		glGenVertexArrays(1, &test);

		// Testing Assert
		BOBO_ASSERT(true, "This should print false");

		TestECS();
	}

	Application::

	Application::~Application()
	{

	}

	void Application::Run()
	{
		p_Window->EventLoop();
	}

	// ECS Testing
	class DuplicatableLabelableComponent : public Component
	{
	public:
		DuplicatableLabelableComponent(std::string label) { m_Label = label; }
		void LogSomething() 
		{ 
			std::string log = m_Label.append(" - LogSomething Called");
			BOBO_CORE_INFO(log); 
		}
		bool AllowDuplicatesOfType() { return true; }
	private:
		std::string m_Label;
	};

	class NonduplicatableLabelableComponent : public Component
	{
	public:
		NonduplicatableLabelableComponent(std::string label) { m_Label = label; }
		void LogSomething() 
		{ 
			std::string log = m_Label.append(" - LogSomething Called");
			BOBO_CORE_INFO(log);
		}
	private:
		std::string m_Label;
	};

	void Application::TestECS() 
	{
		int count;

		// Testing DL Component
		BOBO_CORE_INFO("Testing DuplicatableLabelableComponent");
		Entity* e = new Entity();
		DuplicatableLabelableComponent* dup1 = new DuplicatableLabelableComponent("DuplicatableLabelableComponent 1");
		DuplicatableLabelableComponent* dup2 = new DuplicatableLabelableComponent("DuplicatableLabelableComponent 2");
		e->AddComponent(dup1);
		e->AddComponent(dup2);
		count = e->GetComponents<DuplicatableLabelableComponent>().size();
		BOBO_ASSERT(count == 2, "The number of Added Elements is Incorrect after Adding");

		e->RemoveComponent(dup1);
		count = e->GetComponents<DuplicatableLabelableComponent>().size();
		BOBO_ASSERT(count == 1, "The number of Added Elements is Incorrect after Attempted Removal");

		e->RemoveComponent(dup2);
		count = e->GetComponents<DuplicatableLabelableComponent>().size();
		BOBO_ASSERT(count == 0, "The number of Added Elements is Incorrect after Attempted Removal");

		bool doesNotHaveComponentOfType = !e->HasComponentOfType<DuplicatableLabelableComponent>();
		BOBO_ASSERT(doesNotHaveComponentOfType, "DuplicatableLabelableComponent Component still found on Entity");

		e->AddComponent(dup1);
		e->AddComponent(dup2);
		e->AddComponent(new DuplicatableLabelableComponent("DuplicatableLabelableComponent 3"));
		BOBO_ASSERT(count == 3, "The number of Added Elements is Incorrect after Adding");

		e->AddComponent(dup1);
		BOBO_ASSERT(count == 4, "Added a duplicate Component (pointer)");
		BOBO_ASSERT(e->HasComponentOfType<DuplicatableLabelableComponent>(), "DuplicatableLabelableComponent Component Missing from Entity");

		bool isIndexedCorrectly = e->GetComponent<DuplicatableLabelableComponent>() == dup1;
		BOBO_ASSERT(isIndexedCorrectly, "Recieved Incorrect Component from Get (non-indexed)")
		isIndexedCorrectly = e->GetComponent<DuplicatableLabelableComponent>(1) == dup2;
		BOBO_ASSERT(isIndexedCorrectly, "Recieved Incorrect Component from Get (indexed)")

		e->GetComponent<DuplicatableLabelableComponent>()->LogSomething();
		e->GetComponent<DuplicatableLabelableComponent>(1)->LogSomething();
		e->GetComponent<DuplicatableLabelableComponent>(2)->LogSomething();

		// Testing Non-DL Component
		BOBO_CORE_INFO("Testing Non-DuplicatableLabelableComponent");
		e->AddComponent(new NonduplicatableLabelableComponent("NonduplicatableLabelableComponent 1"));
		e->AddComponent(new NonduplicatableLabelableComponent("NonduplicatableLabelableComponent 2"));
		count = e->GetComponents<NonduplicatableLabelableComponent>().size();
		BOBO_ASSERT(count == 1, "Incorrectly added additional Component of Type")

		e->GetComponent<NonduplicatableLabelableComponent>()->LogSomething();
		e->GetComponent<NonduplicatableLabelableComponent>(1);	
	}
}