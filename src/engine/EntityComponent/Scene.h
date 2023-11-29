#pragma once

#include "../../bpch.h"

#include "EntityManager.h"
#include "Component.h"
#include "BaseComponents/Parent.h"

class Scene
{
public:
	Scene(const std::string& name) : m_SceneName(name) {}

	Entity CreateEntity()
	{
		return EntityManager::CreateEntity();
	}

	Entity CreateEntity(const Entity& parent)
	{
		auto entityId = EntityManager::CreateEntity();
		SetParent(entityId, parent);
		return entityId;
	}

	void DestroyEntity(const Entity& entity)
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
	}

	std::vector<Entity> GetChildren(const Entity& parent)
	{
		auto children = m_ChildMap.find(parent);
		if (children == m_ChildMap.end())
		{
			return {};
		}

		return children->second;
	}

	void SetParent(const Entity& child, const Entity& parent)
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
		} else
		{
			AddComponent<Parent>(child, parent);
		}

		m_ChildMap[parent].push_back(child);
	}


	template <typename T, typename ...Args>
	bool AddComponent(const Entity& entity, Args&&... args)
	{
		if (std::is_base_of<Component, T>::value)
		{
			T* component = new T(std::forward<Args>(args)...);

			// Dynamic cast to Component* so we can call SetOwner, best way to do this...
			Component* base = dynamic_cast<Component*>(component);
			if (base != nullptr)
			{
				base->SetOwner(entity);
			}

			m_ComponentStore[std::type_index(typeid(T))][entity] = component;
			return true;
		}

		return false;
	}

	template <typename T>
	T* GetComponent(const Entity& entity)
	{
		auto componentGroup = m_ComponentStore.find(std::type_index(typeid(T)));
		if (componentGroup == m_ComponentStore.end())
		{
			return nullptr;
		}

		auto component = componentGroup->second.find(entity);
		if (component == componentGroup->second.end())
		{
			return nullptr;
		}

		return static_cast<T*>(component->second);
		// Can also just do return static_cast<T*>(m_ComponentStore[std::type_index(typeid(T))][entity]);
		// It returns nullptr if nothing exists, but it also creates an empty entry there
		// So not using it to avoid pointless memory allocation	
	}

	template <typename T>
	std::vector<T*> GetComponentsOfType()
	{
		auto componentGroup = m_ComponentStore.find(std::type_index(typeid(T)));
		if (componentGroup == m_ComponentStore.end())
		{
			// Return empty vector
			return {};
		}

		std::vector<T*> components;
		for (auto& c : componentGroup->second)
		{
			components.push_back(static_cast<T*>(c.second));
		}

		return components;
	}

	void Awake()
	{
		std::vector<Component*> components = GetAllComponents();
		for (auto& component : components)
		{
			component->Awake();
		}
	}

	void Update()
	{
		std::vector<Component*> components = GetAllComponents();
		for (auto& component : components)
		{
			component->Update();
		}
	}

	void FixedUpdate()
	{
		std::vector<Component*> components = GetAllComponents();
		for (auto& component : components)
		{
			component->FixedUpdate();
		}
	}

	std::string m_SceneName;

private:

	std::map<Entity, std::vector<Entity>> m_ChildMap;
	std::map<std::type_index, std::map<Entity, Component*>> m_ComponentStore;

	std::vector<Component*> GetAllComponents()
	{
		std::vector<Component*> components;
		for (auto& compType : m_ComponentStore)
		{
			for (auto& component : compType.second)
			{
				components.push_back(component.second);
			}
		}
		return components;
	}
};