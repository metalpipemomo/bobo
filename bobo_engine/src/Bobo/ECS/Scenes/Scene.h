#pragma once

#include "bobopch.h"

#include "../EntityManager.h"
#include "../Component.h"
#include "../BaseComponents/Parent.h"
#include "../BaseComponents/Transform.h"
#include "../BaseSystems/PhysicsSystem.h"
#include "../BaseSystems/RendererSystem.h"

namespace Bobo
{
	class BOBO_API Scene
	{
	public:
		Scene(std::string);

		Entity CreateEntity();

		Entity CreateEntity(const Entity& parent);

		void DestroyEntity(const Entity& entity);

		void SetParent(const Entity& child, const Entity& parent);

		std::vector<Entity> GetChildren(const Entity& parent);

		void UpdateSystems();

		void FixedUpdateSystems();

		void AddSystem(System* sys);

		PhysicsSystem* GetPhysicsSystem();

		RendererSystem* GetRendererSystem();

		template <typename T>
		T* GetSystem() {
			for each (System* var in m_Systems)
			{
				if (var == nullptr) continue;

				T* sys = static_cast<T*>(var);
				if (typeid(*sys) == typeid(T)) {
					BOBO_INFO("Got System {}", typeid(T).name());
					return sys;
				}
			}
			BOBO_ERROR("Failed to Get System {}", typeid(T).name());
			return nullptr;
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

	private:
		std::string m_SceneName;
		std::size_t m_EntityCount;
		EntityManager* p_EntityManager;
		std::map<Entity, std::vector<Entity>> m_ChildMap;
		std::map<std::type_index, std::map<Entity, Component*>> m_ComponentStore;
		std::vector<System*> m_Systems;
	};
}