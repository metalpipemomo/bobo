#pragma once
// going to need an update function
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


// Disable common warnings triggered by Jolt, you can use JPH_SUPPRESS_WARNING_PUSH / JPH_SUPPRESS_WARNING_POP to store and restore the warning state
JPH_SUPPRESS_WARNINGS

// All Jolt symbols are in the JPH namespace
using namespace JPH;

// Layer that objects can be in, determines which other objects it can collide with
// Typically you at least want to have 1 layer for moving bodies and 1 layer for static bodies, but you can have more
// layers if you want. E.g. you could have a layer for high detail collision (which is not used by the physics simulation
// but only if you do collision testing).
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


// Each broadphase layer results in a separate bounding volume tree in the broad phase. You at least want to have
// a layer for non-moving and moving objects to avoid having to update a tree full of static objects every frame.
// You can have a 1-on-1 mapping between object layers and broadphase layers (like in this case) but if you have
// many object layers you'll be creating many broad phase trees, which is not efficient. If you want to fine tune
// your broadphase layers define JPH_TRACK_BROADPHASE_STATS and look at the stats reported on the TTY.
namespace BroadPhaseLayers
{
	static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
	static constexpr JPH::BroadPhaseLayer MOVING(1);
	static constexpr JPH::uint NUM_LAYERS(2);
};

// BroadPhaseLayerInterface implementation
// This defines a mapping between object and broadphase layers.
class BPLayerInterfaceImpl final : public BroadPhaseLayerInterface
{
public:
	BPLayerInterfaceImpl()
	{
		// Create a mapping table from object to broad phase layer
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

// An example contact listener
class MyContactListener : public JPH::ContactListener
{
public:
	// See: ContactListener
	virtual JPH::ValidateResult OnContactValidate(const JPH::Body& inBody1, const JPH::Body& inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult& inCollisionResult) override
	{
		std::cout << "Contact validate callback" << std::endl;

		// Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
		return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
	}

	virtual void OnContactAdded(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) override
	{
		std::cout << "A contact was added" << std::endl;
	}

	virtual void OnContactPersisted(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) override
	{
		std::cout << "A contact was persisted" << std::endl;
	}

	virtual void OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair) override
	{
		std::cout << "A contact was removed" << std::endl;
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

// An example activation listener
class MyBodyActivationListener : public JPH::BodyActivationListener
{
public:
	virtual void OnBodyActivated(const JPH::BodyID& inBodyID, JPH::uint64 inBodyUserData) override
	{
		std::cout << "A body got activated" << std::endl;
	}

	virtual void OnBodyDeactivated(const JPH::BodyID& inBodyID, JPH::uint64 inBodyUserData) override
	{
		std::cout << "A body went to sleep" << std::endl;
	}
};



// STL includes
class Physics
{
public:
	static void Init() 
	{
		PhysicsWorld = new Physics();
	};

	static Physics* GetInstance() { return PhysicsWorld; }
	
	JPH::PhysicsSystem* GetPhysicsSystem() { return physics_system; }
	
	
	static void Update() 
	{
		TempAllocatorImpl temp_allocator(10 * 1024 * 1024);

		// We need a job system that will execute physics jobs on multiple threads. Typically
		// you would implement the JobSystem interface yourself and let Jolt Physics run on top
		// of your own job scheduler. JobSystemThreadPool is an example implementation.
		JobSystemThreadPool job_system(cMaxPhysicsJobs, cMaxPhysicsBarriers, thread::hardware_concurrency() - 1);
		auto &body_interface = Physics::PhysicsWorld->physics_system->GetBodyInterface();
		// Output current position and velocity of the sphere
		//RVec3 position = body_interface.GetCenterOfMassPosition(rbid);
		//Vec3 velocity = body_interface.GetLinearVelocity(rbid);
		//std::cout << "Step " << 0 << ": Position = (" << position.GetX() << ", " << position.GetY() << ", " << position.GetZ() << "), Velocity = (" << velocity.GetX() << ", " << velocity.GetY() << ", " << velocity.GetZ() << ")" << std::endl;

		// If you take larger steps than 1 / 60th of a second you need to do multiple collision steps in order to keep the simulation stable. Do 1 collision step per 1 / 60th of a second (round up).
		const int cCollisionSteps = 1;

		// code to display active body positions.
		auto body_id_vector = JPH::BodyIDVector();
		Physics::PhysicsWorld->physics_system->GetActiveBodies(JPH::EBodyType::RigidBody,body_id_vector);
		BodyInterface &interface = Physics::PhysicsWorld->physics_system->GetBodyInterface();
		for(auto body : body_id_vector) 
		{
			RVec3 position = interface.GetCenterOfMassPosition(body);
			Vec3 velocity = interface.GetLinearVelocity(body);
			//std::cout << "Position = (" << position.GetX() << ", " << position.GetY() << ", " << position.GetZ() << "), Velocity = (" << velocity.GetX() << ", " << velocity.GetY() << ", " << velocity.GetZ() << ")" << std::endl;
		}
		
		// Step the world
		Physics::PhysicsWorld->physics_system->Update((1.0/60.0), cCollisionSteps, &temp_allocator, &job_system);
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
		// johnnys stuff/ /
    	// Register allocation hook
		RegisterDefaultAllocator();


		// Create a factory
		Factory::sInstance = new Factory();

		// Register all Jolt physics types
		RegisterTypes();

		// We need a temp allocator for temporary allocations during the physics update. We're
		// pre-allocating 10 MB to avoid having to do allocations during the physics update.
		// B.t.w. 10 MB is way too much for this example but it is a typical value you can use.
		// If you don't want to pre-allocate you can also use TempAllocatorMalloc to fall back to
		// malloc / free.
		TempAllocatorImpl temp_allocator(10 * 1024 * 1024);

		// We need a job system that will execute physics jobs on multiple threads. Typically
		// you would implement the JobSystem interface yourself and let Jolt Physics run on top
		// of your own job scheduler. JobSystemThreadPool is an example implementation.
		JobSystemThreadPool job_system(cMaxPhysicsJobs, cMaxPhysicsBarriers, thread::hardware_concurrency() - 1);

		// This is the max amount of rigid bodies that you can add to the physics system. If you try to add more you'll get an error.
		// Note: This value is low because this is a simple test. For a real project use something in the order of 65536.
		const uint cMaxBodies = 1024;

		// This determines how many mutexes to allocate to protect rigid bodies from concurrent access. Set it to 0 for the default settings.
		const uint cNumBodyMutexes = 0;

		// This is the max amount of body pairs that can be queued at any time (the broad phase will detect overlapping
		// body pairs based on their bounding boxes and will insert them into a queue for the narrowphase). If you make this buffer
		// too small the queue will fill up and the broad phase jobs will start to do narrow phase work. This is slightly less efficient.
		// Note: This value is low because this is a simple test. For a real project use something in the order of 65536.
		const uint cMaxBodyPairs = 1024;

		// This is the maximum size of the contact constraint buffer. If more contacts (collisions between bodies) are detected than this
		// number then these contacts will be ignored and bodies will start interpenetrating / fall through the world.
		// Note: This value is low because this is a simple test. For a real project use something in the order of 10240.
		const uint cMaxContactConstraints = 1024;

		// Create mapping table from object layer to broadphase layer
		// Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!
		broad_phase_layer_interface = new BPLayerInterfaceImpl{};

		// Create class that filters object vs broadphase layers
		// Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!
		object_vs_broadphase_layer_filter = new ObjectVsBroadPhaseLayerFilterImpl{};

		// Create class that filters object vs object layers
		// Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!
		object_vs_object_layer_filter = new ObjectLayerPairFilterImpl{};

		// Now we can create the actual physics system.
		physics_system = new PhysicsSystem{};
		physics_system->Init(cMaxBodies, cNumBodyMutexes, cMaxBodyPairs, cMaxContactConstraints, *broad_phase_layer_interface, *object_vs_broadphase_layer_filter, *object_vs_object_layer_filter);

		// A body activation listener gets notified when bodies activate and go to sleep
		// Note that this is called from a job so whatever you do here needs to be thread safe.
		// Registering one is entirely optional.
		body_activation_listener = new MyBodyActivationListener{};
		physics_system->SetBodyActivationListener(body_activation_listener);

		// A contact listener gets notified when bodies (are about to) collide, and when they separate again.
		// Note that this is called from a job so whatever you do here needs to be thread safe.
		// Registering one is entirely optional.

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

	// We need a job system that will execute physics jobs on multiple threads. Typically
	// you would implement the JobSystem interface yourself and let Jolt Physics run on top
	// of your own job scheduler. JobSystemThreadPool is an example implementation.
	//listeners for when things collide? These are apparently entirely optional 
	MyBodyActivationListener *body_activation_listener;

	std::map<BodyID, Entity> ecsJoltBodyHolder;
};