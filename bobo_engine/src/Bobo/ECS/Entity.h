#pragma once

#include "bobopch.h"

namespace Bobo
{
	class Component;
	class Entity
	{
	public:
		template <typename T>
		void AddComponent(T* component);

		template <typename T>
		T* GetComponent() const;

		template <typename T>
		bool HasComponent() const;

	private:
		std::unordered_map<std::type_index, Component*> components;
	};

	#include "Component.h"

	template <typename T>
	void Entity::AddComponent(T* component)
	{
		// if the component already exists on Entity, log an Error and return early
		if (components.count(typeid(T)) > 0)
		{
			BOBO_CORE_ERROR("Attempted to Add a Component that already exists on Entity");
			return;
		}
		// Insert Component as we're aware it for sure does not already exist on Entity
		components.insert({ typeid(T), component });
	}

	template <typename T>
	T* Entity::GetComponent() const
	{
		auto it = components.find(typeid(T));

		// if the Component requested was not found on the Entity, return a nullptr
		if (it == components.end())
		{
			BOBO_CORE_WARN("Attempted to Get a Component that does not exist on Entity");
			return nullptr;
		}
		else {
			// the Component requested was found, return it
			return static_cast<T*>(it->second);
		}
	}

	template <typename T>
	bool Entity::HasComponent() const
	{
		auto it = components.find(typeid(T));
		return it != components.end();
	}
}