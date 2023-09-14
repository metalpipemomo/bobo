#pragma once

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

	class Component
	{
	public:
		virtual ~Component() {}
		virtual void Awake() {}
		virtual void Update() {}
		virtual void FixedUpdate() {}

	protected:
		Entity* p_Parent;
	};
}