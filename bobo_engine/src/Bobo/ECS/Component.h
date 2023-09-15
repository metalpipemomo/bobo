#pragma once

class Entity;
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