#pragma once

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
#include <GLFW/glfw3.h>

class Rigidbody : public Component
{
public:
	//rigidbody constructor, initiates all precursor rigidbody settings and adds it to the physics system
	Rigidbody(const JPH::Shape* inShape = new JPH::SphereShape(.5), glm::vec3 inPosition = glm::vec3(0.0f, 0.0f, 0.0f),
		JPH::QuatArg inRotation = Quat::sIdentity(), JPH::EMotionType inMotionType = EMotionType::Dynamic,
		JPH::ObjectLayer inObjectLayer = Layers::MOVING, Transform* trnsfrm = nullptr, bool isTrigger = false)
	{
		JPH::BodyCreationSettings shape_settings(inShape, JPH::Vec3(inPosition.x, inPosition.y, inPosition.z), inRotation, inMotionType, inObjectLayer);
		
		shape_settings.mIsSensor = isTrigger;
		
		auto body = Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterfaceNoLock().CreateBody(shape_settings);
		if(inMotionType == EMotionType::Static) {
			Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterfaceNoLock().AddBody(body->GetID(), EActivation::DontActivate);
		} else {
			body->GetMotionProperties()->SetAngularDamping(0.75);
			body->GetMotionProperties()->SetLinearDamping(0.2);
			Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterfaceNoLock().AddBody(body->GetID(), EActivation::Activate);

		}
		m_id = body->GetID();
		transform = trnsfrm;
		// setting physics settings
		PhysicsSettings settings = PhysicsSettings();
		settings.mMinVelocityForRestitution = 0;
		Physics::GetInstance()->GetPhysicsSystem()->SetPhysicsSettings(settings);
	}


	// Rigidbody destructor, completely removes rigidbody from physics system.
	~Rigidbody() 
	{
		Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterfaceNoLock().DestroyBody(m_id);
		Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterfaceNoLock().RemoveBody(m_id);
	}

	bool IsEnabled() {
		return !m_disabled;
	}

	//adds rigidbody to the jolt identity holder on awake
	void Awake() 
	{
		Physics::GetInstance()->SetJoltRbEntity(m_id, m_OwnerId);
	}


	// grabs position data from physics system every update call, updates position and rotation data if transform is not null
	void Update() 
	{
		if(transform != nullptr && !m_disabled) 
		{
			auto tempPosition = Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterfaceNoLock().GetPosition(m_id);
			auto tempRotation = Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterfaceNoLock().GetRotation(m_id).GetEulerAngles();

			transform->position = glm::vec3(tempPosition.GetX(), tempPosition.GetY(), tempPosition.GetZ());
			transform->rotation = glm::vec3(tempRotation.GetX(), tempRotation.GetY(), tempRotation.GetZ());
		}
		if (transform != nullptr && m_disabled) {
			SetPosition(JPH::Vec3{ 100,100,100 });
		}
	}


	// Enables and disables the disabled boolean, used for removing balls from the game
	void EnableBody()
	{
		m_disabled = false;
	}

	void DisableBody() 
	{
		m_disabled = true;
	}
	///

	// Sets the transform variable for the Rigidbody
	void SetTransform(Transform* t) 
	{
		transform = t;
	}
	
	//Adds a linear velocity to the rigidbody in the direction of the vector, speed is determined by how large the number is
	void AddLinearVelocity(Vec3 velocity) 
	{
		Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterfaceNoLock().AddLinearVelocity(m_id, velocity);
	}

	// Gets current position of Rigidbody
	JPH::Vec3 GetPosition() 
	{
		return Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterfaceNoLock().GetCenterOfMassPosition(m_id);
	}

	//Sets the position of the Rigidbody
	void SetPosition(JPH::Vec3 position) 
	{
		Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterfaceNoLock().SetPosition(m_id, position, EActivation::Activate);
	}

	// sets the position while resetting velocity
	void SetPositionHard(JPH::Vec3 position) 
	{
		Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterfaceNoLock().SetPosition(m_id, position, EActivation::Activate);
		SetVelocity(JPH::Vec3::sZero());
	}
	

	//returns current velocity of rigidbody as a vector
	JPH::Vec3 GetVelocity() 
	{
		return Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterfaceNoLock().GetLinearVelocity(m_id);
	}

	//sets current velocity of rigidbody
	void SetVelocity(JPH::Vec3 velocity) 
	{
		Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterfaceNoLock().SetLinearVelocity(m_id, velocity);
	}
	
	//returns friction of rigidbody
	float GetFriction() 
	{
		return Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterfaceNoLock().GetFriction(m_id);
	}

	//sets friction of rigidbody, can be a value between 0.0 - 1.0
	void SetFriction(float friction) 
	{
		Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterfaceNoLock().SetFriction(m_id, friction);
	}

	//returns the bounce value of a rigidbody
	float GetBounce() 
	{
		return Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterfaceNoLock().GetRestitution(m_id);
	}


	// sets the bounciness of a rigidbody, can be a value between 0.0-1.0
	void SetBounce(float bounce) 
	{
		Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterfaceNoLock().SetRestitution(m_id, bounce);
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

	// toggles the motion type of a rigidbody between static and non static, based on the bool parameter "isStatic"
	void SetMotionType(bool isStatic) 
	{
		if(isStatic) 
		{
			Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterfaceNoLock().SetMotionType(m_id, EMotionType::Static, EActivation::Activate);
		} else 
		{
			Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterfaceNoLock().SetMotionType(m_id, EMotionType::Dynamic, EActivation::Activate);
		}
	}

	// set a function to be called when this object continues a collision with another object.
	// the function to be called takes 1 parameter, which is the BodyID of the other object
	void SetOnCollisionPersist(function<void(BodyID)> callback)
	{
		Physics::GetInstance()->m_ContactListener->SetOnCollisionPersistListener(m_id, callback);
	}

	//adds impuse force to the rigidbody.
	void addForce(JPH::Vec3 force)
	{
		Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterfaceNoLock().AddImpulse(m_id, force);
	}

	// returns body id 
	BodyID GetBodyID() 
	{
		return m_id;
	}

	//returns transform
	Transform* GetTransform() 
	{
		return transform;
	}


	
private: 
	JPH::BodyID m_id;
	Transform* transform;
	bool m_disabled = false;
};