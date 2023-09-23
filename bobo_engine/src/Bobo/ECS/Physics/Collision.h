#pragma once

#include "Bobo/ECS/BaseComponents/RigidBody.h"
#include "Bobo/ECS/BaseComponents/Collider.h"
#include "Bobo/ECS/BaseComponents/Transform.h"

namespace Bobo
{
	
	struct Collision
	{
		Transform* transformA;
		RigidBody* rbA;
		Transform* transformB;
		RigidBody* rbB;
		CollisionPoints points;
	};

}