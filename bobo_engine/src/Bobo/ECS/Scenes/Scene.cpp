#include "bobopch.h"

#include "Scene.h"

namespace Bobo 
{
	Scene::Scene(std::string name)
	{
		m_SceneName = name;
		m_EntityCount = 0;
		p_EntityManager = new EntityManager();

		// initialize neccessary Systems
		// Renderer & Physics
		RendererSystem* renderer = new RendererSystem(name);
		PhysicsSystem* physics = new PhysicsSystem(name);
		m_Systems.push_back(renderer);
		m_Systems.push_back(physics);
	}

	Entity Scene::CreateEntity()
	{
		BOBO_INFO("Entity Count in {} (Scene) is Now {}", m_SceneName, ++m_EntityCount);
		return p_EntityManager->CreateEntity();
	}

	Entity Scene::CreateEntity(const Entity& parent)
	{
		BOBO_INFO("Entity Count in {} (Scene) is Now {}", m_SceneName, ++m_EntityCount);
		auto entityId = p_EntityManager->CreateEntity();
		SetParent(entityId, parent);
		return entityId;
	}

	void Scene::DestroyEntity(const Entity& entity)
	{
		auto children = GetChildren(entity);

		for (auto& child : children)
		{
			DestroyEntity(child);
		}

		for (auto& componentGroup : m_ComponentStore)
		{
			componentGroup.second.erase(entity);
		}

		BOBO_INFO("Entity Count in {} (Scene) is Now {}", m_SceneName, --m_EntityCount);
	}

	void Scene::SetParent(const Entity& child, const Entity& parent)
	{
		auto oldParentComponent = GetComponent<Parent>(child);

		if (oldParentComponent != nullptr)
		{
			auto& children = m_ChildMap[oldParentComponent->parentId];

			for (size_t i = 0; i < children.size(); ++i)
			{
				if (child == children[i])
				{
					children.erase(children.begin() + i);
				}
			}
			oldParentComponent->parentId = parent;
			return;
		}
		else
		{
			AddComponent<Parent>(child, parent);
		}

		m_ChildMap[parent].push_back(child);
	}

	std::vector<Entity> Scene::GetChildren(const Entity& parent)
	{
		auto children = m_ChildMap.find(parent);
		if (children == m_ChildMap.end())
		{
			return {};
		}

		return children->second;
	}

	void Scene::UpdateSystems()
	{
		for (System* sys : m_Systems)
		{
			sys->Update();
		}
	}

	void Scene::FixedUpdateSystems()
	{
		for (System* sys : m_Systems)
		{
			sys->FixedUpdate();
		}
	}

	void Scene::AddSystem(System* sys)
	{
		BOBO_INFO("Added System {}", typeid(*sys).name());
		m_Systems.push_back(sys);
	}

	PhysicsSystem* Scene::GetPhysicsSystem()
	{
		System* sys = GetSystem<PhysicsSystem>();
		if (sys == nullptr) return nullptr;
		return static_cast<PhysicsSystem*>(sys);
	}

	RendererSystem* Scene::GetRendererSystem()
	{
		System* sys = GetSystem<RendererSystem>();
		if (sys == nullptr) return nullptr;
		return static_cast<RendererSystem*>(sys);
	}
}