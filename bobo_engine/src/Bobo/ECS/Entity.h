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
		void RemoveComponent(T* component);

		template <typename T>
		T* GetComponent(size_t index) const;

		template <typename T>
		T* GetComponent() const;

		template <typename T>
		std::vector<Component*> GetComponents() const;

		template <typename T>
		bool HasComponentOfType() const;

	private:
		std::unordered_map<std::type_index, std::vector<Component*>> m_ComponentsMap;
	};

	#include "Component.h"

	template <typename T>
	void Entity::AddComponent(T* component)
	{
		auto it = m_ComponentsMap.find(typeid(T));

		// if the ComponentMap doesn't already contains the given Component, we may simply insert a 
		// new vector into the map with the given Component
		if (it == m_ComponentsMap.end())
		{
			m_ComponentsMap.insert({ typeid(T), std::vector<Component*>({ component }) });
			BOBO_CORE_INFO("Added Component to New Entry in Component Map");
		}
		else 
		{
			// Otherwise, there is a pre-existing Vector that we must look at
			// if there is already an existing Component of the given type AND it is a component that does not Allow for multiple
			// of the same Type of Component, we Log an Error and do NOT add the Component
			if (it->second.size() > 0 && !component->AllowDuplicatesOfType())
			{
				BOBO_CORE_ERROR("Attempted to Add a Component of a Type that already Exists on Entity but doesn't Allow Duplicates of Type");
			}
			else
			{
				// Otherwise, we check to ensure that the Component Pointer we are trying to add is not already contained within the Vector
				auto vIt = std::find(it->second.begin(), it->second.end(), component);
				if (vIt == it->second.end())
				{
					// The given Component is not found in the vector, thus it is a brand new pointer and we can Add it safely
					it->second.push_back(component);
					BOBO_CORE_INFO("Added Component to Existing Vector");
				}
				else
				{
					// The given Component was found already in the vector, Log an Error and do NOT add the Component
					BOBO_CORE_ERROR("Attempted to Add a Component that already Exists on Entity");
				}
			}
		}
	}

	template<typename T>
	 void Entity::RemoveComponent(T* component)
	{
		auto it = m_ComponentsMap.find(typeid(T));

		if (it == m_ComponentsMap.end()) 
		{
			// The given Component was not found in the ComponentsMap, Log a Warning
			BOBO_CORE_WARN("Attempted to Remove a Component that does not Exist on Entity");
		}
		else 
		{
			auto vIt = std::find(it->second.begin(), it->second.end(), component);
			if (vIt != it->second.end()) 
			{
				// The given Component was found in the ComponentsMap and in the associated Vector, remove it from the Vector
				it->second.erase(vIt);
				BOBO_CORE_INFO("Successfully Removed Component");
			}
			else 
			{
				// The given Component was found in the ComponentsMap but not in the associated Vector, Log a Warning
				BOBO_CORE_ERROR("Attempted to Remove a Component that does not Exist on Entity");
			}
		}
	}

	template <typename T>
	T* Entity::GetComponent(size_t index) const
	{
		auto it = m_ComponentsMap.find(typeid(T));

		// if the given Component was found in the ComponentsMap, we'll return the first element in the respective vector
		// if the vector has no elements, Log a Warning and return a nullptr
		if (it != m_ComponentsMap.end() && it->second.size() > 0) 
		{
			// if the index specified is larger than the number of that Component on the Entity, Log a Warning and return a nullptr
			if (index > it->second.size() - 1)
			{
				BOBO_CORE_ERROR("Attempted to Get a Component from an Index Larger than the size of the Vector");
				return nullptr;
			}
			else
			{
				// Success Case!
				return static_cast<T*>(it->second[index]);
			}
		}
		else 
		{
			// otherwise, Log a Warning and return a nullptr
			BOBO_CORE_ERROR("Attempted to Get a Component that does not Exist on Entity");
			return nullptr;
		}
	}

	template<typename T>
	T* Entity::GetComponent() const
	{
		return GetComponent<T>(0);
	}

	template<typename T>
	std::vector<Component*> Entity::GetComponents() const
	{
		auto it = m_ComponentsMap.find(typeid(T));

		// if the given Component was not found in the ComponentsMap, it doesn't exist on this Entity
		// return an empty vector to communicate this
		if (it == m_ComponentsMap.end()) 
		{
			return std::vector<Component*>();
		}
		else 
		{
			// the given Component was found in the ComponentsMap, we can simply return it
			return it->second;
		}
	}

	template <typename T>
	bool Entity::HasComponentOfType() const
	{
		auto it = m_ComponentsMap.find(typeid(T));
		return it != m_ComponentsMap.end() && it->second.size() > 0;
	}
}