#pragma once

#include "EntityManager.h"

class Component
{
public:
	Component() = default;
	virtual ~Component() = default;
	virtual void Awake() {};
	virtual void Update() {};
	virtual void FixedUpdate() {};

	void SetOwner(const Entity& id)
	{
		m_OwnerId = id;
	}

	Entity m_OwnerId;
};