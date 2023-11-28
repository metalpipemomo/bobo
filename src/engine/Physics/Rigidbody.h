#pragma once
#pragma once
#pragma once
// going to need an update function
#include <Jolt/Jolt.h>
#include "../EntityComponent/BaseComponents/Transform.h"

// Jolt includes
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
#include <iostream>
#include "Physics.h"

// STL includes
class Rigidbody : public Component
{
public:
	void Init() 
	{
		
	}

	void Update() 
	{
		if(transform != nullptr) 
		{
			auto tempPosition = Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterface().GetPosition(m_id);
			auto tempRotation = Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterface().GetRotation(m_id);
			transform->position = glm::vec3(tempPosition.GetX(), tempPosition.GetY(), tempPosition.GetZ());
			transform->rotation = glm::vec3(tempRotation.GetX(), tempRotation.GetY(), tempRotation.GetZ());

		}
	}

	void SetTransform(Transform* t) 
	{
		transform = t;
	}

	Rigidbody(const JPH::Shape *inShape = new JPH::SphereShape(.5), JPH::RVec3Arg inPosition = JPH::RVec3Arg(0.0f,0.0f,0.0f), 
		JPH::QuatArg inRotation = Quat::sIdentity(), JPH::EMotionType inMotionType = EMotionType::Dynamic, 
		JPH::ObjectLayer inObjectLayer = Layers::MOVING, Transform *trnsfrm = nullptr) 
	{
	
		JPH::BodyCreationSettings shape_settings(inShape, inPosition, inRotation,inMotionType,inObjectLayer);
		m_id = Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterface().CreateAndAddBody(shape_settings, EActivation::Activate);
		transform = trnsfrm;
	}
	
	void AddLinearVelocity(Vec3 velocity) 
	{
		Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterface().AddLinearVelocity(m_id, velocity);
	}

	JPH::Vec3 GetPosition() 
	{
		return Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterface().GetCenterOfMassPosition(m_id);
	}

	void SetPosition(JPH::Vec3 position) 
	{
		Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterface().SetPosition(m_id, position, EActivation::Activate);
	}

	// sets the position while resetting velocity
	void SetPositionHard(JPH::Vec3 position) 
	{
		Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterface().SetPosition(m_id, position, EActivation::Activate);
		SetVelocity(JPH::Vec3::sZero());
	}
	
	JPH::Vec3 GetVelocity() 
	{
		return Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterface().GetLinearVelocity(m_id);
	}

	void SetVelocity(JPH::Vec3 velocity) 
	{
		Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterface().SetLinearVelocity(m_id, velocity);
	}

	float GetFriction() 
	{
		return Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterface().GetFriction(m_id);
	}

	void SetFriction(float friction) 
	{
		Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterface().SetFriction(m_id, friction);
	}

	float GetBounce() 
	{
		return Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterface().GetRestitution(m_id);
	}

	void SetBounce(float bounce) 
	{
		Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterface().SetRestitution(m_id, bounce);
	}

	void SetOnCollision(function<void(BodyID)> callback)
	{
		Physics::GetInstance()->contactListener->SetOnCollisionListener(m_id, callback);
	}

	void SetOnCollisionEnd(function<void(BodyID)> callback)
	{
		Physics::GetInstance()->contactListener->SetOnCollisionEndListener(m_id, callback);
	}

	void SetOnCollisionPersist(function<void(BodyID)> callback)
	{
		Physics::GetInstance()->contactListener->SetOnCollisionPersistListener(m_id, callback);
	}

	JPH::BodyID m_id;
	Transform* transform;
private: 
};