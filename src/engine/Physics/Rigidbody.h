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
			body->GetMotionProperties()->SetAngularDamping(0.75);
			Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterface().AddBody(body->GetID(), EActivation::Activate);

		}
		m_id = body->GetID();
		transform = trnsfrm;
		// setting physics settings
		PhysicsSettings settings = PhysicsSettings();
		settings.mMinVelocityForRestitution = 0;
		Physics::GetInstance()->GetPhysicsSystem()->SetPhysicsSettings(settings);
	}

	~Rigidbody() 
	{
		Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterface().DestroyBody(m_id);
		Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterface().RemoveBody(m_id);
	}



	void Awake() 
	{
		Physics::GetInstance()->SetJoltRbEntity(m_id, m_OwnerId);
	}

	void Update() 
	{
		if(transform != nullptr && !disabled) 
		{
			auto tempPosition = Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterface().GetPosition(m_id);
			auto tempRotation = Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterface().GetRotation(m_id);
			transform->position = glm::vec3(tempPosition.GetX(), tempPosition.GetY(), tempPosition.GetZ());
			transform->rotation = glm::vec3(tempRotation.GetX(), tempRotation.GetY(), tempRotation.GetZ());
		}
		if (transform != nullptr && disabled) {
			SetPosition(JPH::Vec3{ 100,100,100 });
		}
	}

	void DisableBody() 
	{
		disabled = true;
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

	void SetMotionType(bool isStatic) 
	{
		if(isStatic) 
		{
			Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterface().SetMotionType(m_id, EMotionType::Static, EActivation::Activate);
		} else 
		{
			Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterface().SetMotionType(m_id, EMotionType::Dynamic, EActivation::Activate);
		}
	}

	// set a function to be called when this object continues a collision with another object.
	// the function to be called takes 1 parameter, which is the BodyID of the other object
	void SetOnCollisionPersist(function<void(BodyID)> callback)
	{
		Physics::GetInstance()->m_ContactListener->SetOnCollisionPersistListener(m_id, callback);
	}

	void addForce(JPH::Vec3 force)
	{
		Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterface().AddImpulse(m_id, force);
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
	bool disabled = false;
};