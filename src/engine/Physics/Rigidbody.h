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
	Rigidbody(const JPH::Shape* inShape = new JPH::SphereShape(.5), glm::vec3 inPosition = glm::vec3(0.0f, 0.0f, 0.0f),
		JPH::QuatArg inRotation = Quat::sIdentity(), JPH::EMotionType inMotionType = EMotionType::Dynamic,
		JPH::ObjectLayer inObjectLayer = Layers::MOVING, Transform* trnsfrm = nullptr, bool isTrigger = false)
	{
		JPH::BodyCreationSettings shape_settings(inShape, JPH::Vec3(inPosition.x, inPosition.y, inPosition.z), inRotation, inMotionType, inObjectLayer);
		
		shape_settings.mIsSensor = isTrigger;
		
		auto body = Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterface().CreateBody(shape_settings);
		if(inMotionType == EMotionType::Static) {
			Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterface().AddBody(body->GetID(), EActivation::DontActivate);
		} else {
			Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterface().AddBody(body->GetID(), EActivation::Activate);
			body->GetMotionProperties()->SetLinearDamping(.5);
			body->GetMotionProperties()->SetAngularDamping(.6);

		}
		m_id = body->GetID();
		transform = trnsfrm;
	}



	void Awake() 
	{
		Physics::GetInstance()->SetJoltRbEntity(m_id, m_OwnerId);
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
		SetVelocity(0,0,0);
	}
	
	glm::vec3 GetVelocity() 
	{
		auto temp = Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterface().GetLinearVelocity(m_id); 
		return glm::vec3(temp.GetX(), temp.GetY(), temp.GetZ());
	}

	void SetVelocity(glm::vec3 velocity) 
	{
		auto temp = JPH::Vec3(velocity.x, velocity.y, velocity.z);
		Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterface().SetLinearVelocity(m_id, temp);
		
	}


	void SetVelocity(float VelocityX, float VelocityY, float VelocityZ) 
	{
		auto temp = JPH::Vec3(VelocityX, VelocityY, VelocityZ);
		Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterface().SetLinearVelocity(m_id, temp);
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

	// set a function to be called when this object collides with another object.
	// the function to be called takes 1 parameter, which is the BodyID of the other object
	void SetOnCollision(function<void(BodyID)> callback)
	{
		Physics::GetInstance()->m_ContactListener->SetOnCollisionListener(m_id, callback);
	}

	// set a function to be called when this object is finishes colliding with another object.
	// the function to be called takes 1 parameter, which is the BodyID of the other object
	void SetOnCollisionEnd(function<void(BodyID)> callback)
	{
		Physics::GetInstance()->m_ContactListener->SetOnCollisionEndListener(m_id, callback);
	}

	// set a function to be called when this object continues a collision with another object.
	// the function to be called takes 1 parameter, which is the BodyID of the other object
	void SetOnCollisionPersist(function<void(BodyID)> callback)
	{
		Physics::GetInstance()->m_ContactListener->SetOnCollisionPersistListener(m_id, callback);
	}

	BodyID GetBodyID() 
	{
		return m_id;
	}

	Transform* GetTransform() 
	{
		return transform;
	}
	
private: 
	JPH::BodyID m_id;
	Transform* transform;
};