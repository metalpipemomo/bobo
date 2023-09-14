#include "bobopch.h"
#include "Entity.h"

namespace Bobo
{
	template <typename T>
	void Entity::AddComponent(T* component)
	{
		BOBO_ASSERT(components.count(typeid(T)) > 0, "Tried to add existing component to entity.");

		components.insert({ typeid(T), component });
	}

	template <typename T>
	T* Entity::GetComponent() const
	{
		auto it = components.find(typeid(T));

		BOBO_ASSERT(it != components.end(), "Component does not exist on entity,");

		return static_cast<T*>(it->second);
	}

	template <typename T>
	bool Entity::HasComponent() const
	{
		auto it = components.find(typeid(T));

		return it != components.end();
	}
}