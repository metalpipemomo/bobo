#pragma once

#include "bobopch.h"

#include "EntityManager.h"
#include "System.h"
#include "Component.h"
#include "BaseComponents/Parent.h"
#include "BaseComponents/Transform.h"

namespace Bobo
{
	class Scene
	{
	public:
		Scene()
		{
			p_EntityManager = new EntityManager();
		}

		Entity CreateEntity()
		{
			return p_EntityManager->CreateEntity();
		}

		Entity CreateEntity(const Entity& parent)
		{
			auto entityId = p_EntityManager->CreateEntity();
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

		template <typename T, typename... Args>
		bool AddComponent(const Entity& entity, Args&&... args)
		{
			// Make sure it is a component being added
			if (std::is_base_of<Component, T>::value)
			{
				T* component = new T(std::forward<Args>(args)...);
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

		std::vector<Entity> GetChildren(const Entity& parent)
		{
			auto children = m_ChildMap.find(parent);
			if (children == m_ChildMap.end())
			{
				return {};
			}

			return children->second;
		}

	private:
		EntityManager* p_EntityManager;
		std::map<Entity, std::vector<Entity>> m_ChildMap;
		std::map<std::type_index, std::map<Entity, Component*>> m_ComponentStore;
		std::vector<System*> m_Systems;
	};
}