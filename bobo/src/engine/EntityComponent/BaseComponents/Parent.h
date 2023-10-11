#pragma once

#include "../Component.h"
#include "../EntityManager.h"

class Parent : public Component
{
public:
	Entity parentId;

	Parent(const Entity& id) : parentId(id) {}
};