#pragma once
#include <Jolt/Jolt.h>


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
#include "../EntityComponent/EntityManager.h"
#include "../Time.h"

JPH_SUPPRESS_WARNINGS

using namespace JPH;

/// **** All of this below here is jolt system set up variables required to create a base physics system with jolt,
//  **** game dev people do not concern yourselves.


// Layer that objects can be in, determines which other objects it can collide with
namespace Layers
{
	static constexpr JPH::ObjectLayer NON_MOVING = 0;
	static constexpr JPH::ObjectLayer MOVING = 1;
	static constexpr JPH::ObjectLayer NUM_LAYERS = 2;
};

/// Class that determines if two object layers can collide
class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter
{
public:
	virtual bool ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override
	{
		switch (inObject1)
		{
		case Layers::NON_MOVING:
			return inObject2 == Layers::MOVING; // Non moving only collides with moving
		case Layers::MOVING:
			return true; // Moving collides with everything
		default:
			JPH_ASSERT(false);
			return false;
		}
	}
};


// Broadphase layer distinctions, differentiating between non_moving and moving prevents 
// the bounding volume tree to have to update static objects every frame.
namespace BroadPhaseLayers
{
	static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
	static constexpr JPH::BroadPhaseLayer MOVING(1);
	static constexpr JPH::uint NUM_LAYERS(2);
};

// BroadPhaseLayerInterface implementation
// defines a mapping between object and broadphase layers.
class BPLayerInterfaceImpl final : public BroadPhaseLayerInterface
{
public:
	BPLayerInterfaceImpl()
	{
		mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
		mObjectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;
	}

	virtual uint GetNumBroadPhaseLayers() const override
	{
		return BroadPhaseLayers::NUM_LAYERS;
	}

	virtual BroadPhaseLayer GetBroadPhaseLayer(ObjectLayer inLayer) const override
	{
		JPH_ASSERT(inLayer < Layers::NUM_LAYERS);
		return mObjectToBroadPhase[inLayer];
	}

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
	virtual const char *			GetBroadPhaseLayerName(BroadPhaseLayer inLayer) const override
	{
		switch ((BroadPhaseLayer::Type)inLayer)
		{
		case (BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING:	return "NON_MOVING";
		case (BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:		return "MOVING";
		default:													JPH_ASSERT(false); return "INVALID";
		}
	}
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

private:
	BroadPhaseLayer mObjectToBroadPhase[Layers::NUM_LAYERS];
};

/// Class that determines if an object layer can collide with a broadphase layer
class ObjectVsBroadPhaseLayerFilterImpl : public ObjectVsBroadPhaseLayerFilter
{
public:
	virtual bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override
	{
		switch (inLayer1)
		{
		case Layers::NON_MOVING:
			return inLayer2 == BroadPhaseLayers::MOVING;
		case Layers::MOVING:
			return true;
		default:
			JPH_ASSERT(false);
			return false;
		}
	}
};

// Contact manager that handles events when physics objects come into contact with one another
class MyContactListener : public JPH::ContactListener
{
public:
	// See: ContactListener
	virtual JPH::ValidateResult OnContactValidate(const JPH::Body& inBody1, const JPH::Body& inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult& inCollisionResult) override
	{
		//std::cout << "Contact validate callback" << std::endl;

		// Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
		return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
	}

	virtual void OnContactAdded(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) override
	{
		//std::cout << "A contact was added" << std::endl;
	}

	virtual void OnContactPersisted(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) override
	{
		//std::cout << "A contact was persisted" << std::endl;
	}

	virtual void OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair) override
	{
		//std::cout << "A contact was removed" << std::endl;
	}
};

class ActionableContactListener : public JPH::ContactListener
{
public:
	// set a function to be called when some body starts a collision
	void SetOnCollisionListener(BodyID bodyID, function<void(BodyID)> function) 
	{
		onCollisionListeners[bodyID] = function;
	}

	// set a function to be called when some body finishes a collision
	void SetOnCollisionEndListener(BodyID bodyID, function<void(BodyID)> function) 
	{
		onCollisionEndListeners[bodyID] = function;
	}

	// set a function to be called while some body continues to be in a collision
	void SetOnCollisionPersistListener(BodyID bodyID, function<void(BodyID)> function) 
	{
		onCollisionPersistListeners[bodyID] = function;
	}

	virtual void OnContactAdded(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) override
	{
		BodyID body1ID = inBody1.GetID();
		BodyID body2ID = inBody2.GetID();

		if (onCollisionListeners.find(body1ID) != onCollisionListeners.end()) {
			onCollisionListeners[body1ID](body2ID);
		}

		if (onCollisionListeners.find(body2ID) != onCollisionListeners.end()) {
			onCollisionListeners[body2ID](body1ID);
		}
	}

	virtual void OnContactPersisted(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) override
	{
		BodyID body1ID = inBody1.GetID();
		BodyID body2ID = inBody2.GetID();

		if (onCollisionPersistListeners.find(body1ID) != onCollisionPersistListeners.end()) {
			onCollisionPersistListeners[body1ID](body2ID);
		}

		if (onCollisionPersistListeners.find(body2ID) != onCollisionPersistListeners.end()) {
			onCollisionPersistListeners[body2ID](body1ID);
		}
	}

	virtual void OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair) override
	{
		BodyID body1ID = inSubShapePair.GetBody1ID();
		BodyID body2ID = inSubShapePair.GetBody2ID();

		if (onCollisionEndListeners.find(body1ID) != onCollisionEndListeners.end()) {
			onCollisionEndListeners[body1ID](body2ID);
		}

		if (onCollisionEndListeners.find(body2ID) != onCollisionEndListeners.end()) {
			onCollisionEndListeners[body2ID](body1ID);
		}
	}

private:
	// all of these have a key of the body id of the object whose collision we care about, 
	// and value of a function to be called on collision, with a parameter of the other object
	std::map<BodyID, function<void(BodyID)>> onCollisionListeners;
	std::map<BodyID, function<void(BodyID)>> onCollisionEndListeners;
	std::map<BodyID, function<void(BodyID)>> onCollisionPersistListeners;
};

// **** End of Jolt Set up variables.


class Physics
{
public:
	static void Init() 
	{
		PhysicsWorld = new Physics();
		// setting physics settings
		PhysicsSettings settings = PhysicsSettings();
		settings.mMinVelocityForRestitution = 0;
		PhysicsWorld->GetPhysicsSystem()->SetPhysicsSettings(settings);
	};

	static Physics* GetInstance() { return PhysicsWorld; }
	
	JPH::PhysicsSystem* GetPhysicsSystem() { return physics_system; }
	
	
	static void Update() 
	{
		// Allocates block of memory for physics
		TempAllocatorImpl temp_allocator(10 * 1024 * 1024);

		// Base Job Handler system. why do these things have to be constantly created? it breaks if i try and make it a static variable
		JobSystemThreadPool job_system(cMaxPhysicsJobs, cMaxPhysicsBarriers, thread::hardware_concurrency() - 1);
	
		const int cCollisionSteps = 1;

		// Step the world
		Physics::PhysicsWorld->physics_system->Update((Time::GetTimeScale()/60.0), cCollisionSteps, &temp_allocator, &job_system);
	}

	// use this function if you need to get the entity corresponding to some jolt rigidbody id
	Entity GetEntityFromJoltRb(BodyID id) {
		return ecsJoltBodyHolder.find(id)->second;
	}

	void SetJoltRbEntity(BodyID id, Entity entity) {
		ecsJoltBodyHolder[id] = entity;
	}
	
	ActionableContactListener *m_ContactListener;
private:
	Physics() 
	{
    	// Register allocation hook
		RegisterDefaultAllocator();


		// Create a factory
		Factory::sInstance = new Factory();

		// Register all Jolt physics types
		RegisterTypes();

		// This is the max amount of rigid bodies that you can add to the physics system. If you try to add more you'll get an error.
		const uint cMaxBodies = 1024;

		// This determines how many mutexes to allocate to protect rigid bodies from concurrent access. Set it to 0 for the default settings.
		const uint cNumBodyMutexes = 0;

		// This is the max amount of body pairs that can be queued at any time (the broad phase will detect overlapping
		// body pairs based on their bounding boxes and will insert them into a queue for the narrowphase)
		const uint cMaxBodyPairs = 1024;

		// This is the maximum size of the contact constraint buffer. 
		const uint cMaxContactConstraints = 1024;

		// Create mapping table from object layer to broadphase layer
		broad_phase_layer_interface = new BPLayerInterfaceImpl{};

		// Create class that filters object vs broadphase layers
		object_vs_broadphase_layer_filter = new ObjectVsBroadPhaseLayerFilterImpl{};

		// Create class that filters object vs object layers
		object_vs_object_layer_filter = new ObjectLayerPairFilterImpl{};

		physics_system = new PhysicsSystem{};
		physics_system->Init(cMaxBodies, cNumBodyMutexes, cMaxBodyPairs, cMaxContactConstraints, *broad_phase_layer_interface, *object_vs_broadphase_layer_filter, *object_vs_object_layer_filter);



		// this contact listener is what lets rigidbodies have their own collision-based events
		m_ContactListener = new ActionableContactListener();
		physics_system->SetContactListener(m_ContactListener);
	}

	inline static Physics* PhysicsWorld;
	JPH::PhysicsSystem* physics_system;

	//interface objects for collision detection? still figuring out what these do. all i know is that they have to stay alive
	ObjectVsBroadPhaseLayerFilterImpl *object_vs_broadphase_layer_filter;
	ObjectLayerPairFilterImpl *object_vs_object_layer_filter;
	BPLayerInterfaceImpl *broad_phase_layer_interface;

	std::map<BodyID, Entity> ecsJoltBodyHolder;
};