#include "bobopch.h"

#include "Scene.h"

namespace Bobo 
{
	Scene::Scene(const std::string& name)
	{
		m_SceneName = name;
		m_EntityCount = 0;
		p_EntityManager = new EntityManager();
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
		for (System* system : m_Systems)
		{
			system->Update();
		}
	}

	void Scene::FixedUpdateSystems()
	{
		for (System* system : m_Systems)
		{
			system->FixedUpdate();
		}
	}

	void Scene::AddSystem(System* system)
	{
		BOBO_INFO("Added System {}", typeid(*system).name());
		m_Systems.push_back(system);
	}
}