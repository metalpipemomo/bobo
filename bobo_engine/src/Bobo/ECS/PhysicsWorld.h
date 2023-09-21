// #pragma once
// #include "bobopch.h"
// #include "Vector3.h"
// #include <thread>
// /*
// 	Anyone viewing this code. Stop.

// 	This is just temporary test code for our physics engine.

// 	Chances are this wont work.

// 	- Johnny "Ya boy" Johnson

// */

// struct Collider;
// struct SphereCollider;
// struct PlaneCollider;
// struct Transform
// {
// 	Vector3 position;
// 	Vector3 scale;
// 	Vector3 Rotation;
// };

// struct CollisionPoints
// {
// 	Vector3 A;
// 	Vector3 B;
// 	Vector3 Normal;
// 	float depth;
// 	bool HasCollision;
// };

// struct Collider {
// 	virtual CollisionPoints TestCollision(
// 		const Transform* transform,
// 		const Collider* collider,
// 		const Transform* colliderTransform
// 	) const = 0;
// 	virtual CollisionPoints TestCollision(
// 		const Transform* transform,
// 		const SphereCollider* sphere,
// 		const Transform* sphereTransform
// 	) const = 0;
// 	virtual CollisionPoints TestCollision(
// 		const Transform* transform,
// 		const PlaneCollider* plane,
// 		const Transform* planeTransform
// 	) const = 0;
// };

// struct SphereCollider : Collider {
// 	Vector3 Center;
// 	float Radius;
// 	CollisionPoints TestCollision(
// 		const Transform* transform,
// 		const Collider* collider,
// 		const Transform* colliderTransform) const override
// 	{
// 		return collider->TestCollision(colliderTransform, this, transform);
// 	};

// 	CollisionPoints TestCollision(
// 		const Transform* transform,
// 		const SphereCollider* sphere,
// 		const Transform* sphereTransform) const override
// 	{

// 		Vector3 direction = sphereTransform->position - transform->position; // get direction from point A to point B

// 		float depth = std::sqrt(std::powf(direction.GetX(), 2) + std::powf(direction.GetY(), 2) + std::powf(direction.GetZ(), 2)); // Get length of the direction Vector
// 		BOBO_INFO(std::to_string(depth));
// 		Vector3 normal = direction / depth; // Get Normal of the direction vector

// 		// determine if the objects are colliding
// 		// possibly change this so if they are not collding return null
// 		bool hasCollision = false;
// 		BOBO_INFO(std::to_string(sphere->Radius));

// 		if (depth <= 1 + sphere->Radius)
// 			hasCollision = true;

// 		Vector3 A = transform->position + (direction * this->Radius); //deepest point of sphere a in sphere b
// 		Vector3 B = sphereTransform->position - (direction * this->Radius); //deepest point of sphere b in sphere a
// 		CollisionPoints collisionpoints{ A, B, normal, depth, hasCollision };
// 		return collisionpoints;

// 		//get normal between a and b
// 		// deepest point a will equal transform + radius(in direction of b)
// 		// deepest point b will equal transformb+radiusb(indirection of a);

// 	};

// 	CollisionPoints TestCollision(
// 		const Transform* transform,
// 		const PlaneCollider* plane,
// 		const Transform* planeTransform) const override
// 	{
// 		Vector3 direction = planeTransform->position - transform->position; // get direction from point A to point B

// 		float depth = std::sqrt(std::powf(direction.GetX(), 2) + std::powf(direction.GetY(), 2) + std::powf(direction.GetZ(), 2)); // Get length of the direction Vector

// 		Vector3 normal = direction / depth; // Get Normal of the direction vector

// 		// determine if the objects are colliding
// 		// possibly change this so if they are not collding return null

// 		bool hasCollision = false;
// 		Vector3 temp = planeTransform->position;
// 		Vector3 temp2 = transform->position;
// 		float distanceFromPlane = temp.GetY() - temp2.GetY();;
// 		if (distanceFromPlane < this->Radius)
// 			hasCollision = true;

// 		Vector3 A = transform->position + (direction * this->Radius); //deepest point of sphere a in sphere b
// 		Vector3 B = planeTransform->position - (direction * this->Radius); //deepest point of sphere b in sphere a

// 		CollisionPoints collisionpoints{ A, B, normal, depth, hasCollision };
// 		return collisionpoints;

// 	};

// };

// struct PlaneCollider : Collider {
// 	Vector3 plane;
// 	float distance;

// 	CollisionPoints TestCollision(
// 		const Transform* transform,
// 		const Collider* collider,
// 		const Transform* colliderTransform) const override
// 	{
// 		return collider->TestCollision(colliderTransform, this, transform);
// 	};

// 	CollisionPoints TestCollision(
// 		const Transform* transform,
// 		const SphereCollider* sphere,
// 		const Transform* sphereTransform)  const override
// 	{
// 		return {};

// 	};

// 	CollisionPoints TestCollision(
// 		const Transform* transform,
// 		const PlaneCollider* plane,
// 		const Transform* planeTransform) const override
// 	{
// 		return {};
// 	};
// };

// struct GameObject
// {
// 	float mass;
// 	Vector3 velocity;
// 	Vector3 force;

// 	Collider* Collider;
// 	Transform* transform;

// 	bool hasGravity = true;
// };

// struct Collision {
// 	GameObject* ObjA;
// 	GameObject* ObjB;
// 	CollisionPoints points;
// };

// class PhysicsWorld
// {
// private:
// 	PhysicsWorld() {};

// 	std::vector<GameObject*> m_PhysicsObjects;
// 	Vector3 m_Gravity = Vector3(0, -9.81f, 0);
// 	std::chrono::high_resolution_clock::time_point m_PreviousTime;
// 	std::chrono::high_resolution_clock::time_point m_CurrentTime;
// public:
// 	static PhysicsWorld& GetInstance()
// 	{

// 		static PhysicsWorld* instance = new PhysicsWorld();
// 		return *instance;

// 	}
// 	void AddObject(GameObject* object)
// 	{
// 		m_PhysicsObjects.push_back(object);
// 	}

// 	void RemoveObject(GameObject* object)
// 	{
// 		if (!object) return;
// 		auto itr = std::find(m_PhysicsObjects.begin(), m_PhysicsObjects.end(), object);
// 		if (itr == m_PhysicsObjects.end()) return;
// 		m_PhysicsObjects.erase(itr);
// 	}

 	void Update(float dt)
 	{

 	//	for (GameObject* obj : this->m_PhysicsObjects)
 	//	{
 	//		if (obj->hasGravity) {
 	//			BOBO_INFO("Velocity Y: " + std::to_string(dt));


 	//			obj->force = obj->force + (m_Gravity * obj->mass);

 	//			obj->velocity = obj->velocity + ((obj->force / obj->mass) * dt);
 	//			
  //              obj->transform->position = obj->transform->position + (obj->velocity * dt);




 	//			BOBO_INFO("Velocity Y: " + std::to_string(obj->velocity.GetY()));
 	//			BOBO_INFO("Position Y: " + std::to_string((obj->transform->position).GetY()));

 	//			obj->force = Vector3(0, 0, 0);
 	//		}
 	//	}

 	//	ResolveCollisions(dt);

 	//}

// 	void ResolveCollisions(float dt) {
// 		std::vector<Collision> collisions;

// 		for (GameObject* a : m_PhysicsObjects)
// 		{
// 			for (GameObject* b : m_PhysicsObjects)
// 			{
// 				if (a == b) break;

// 				if (!a->Collider || !b->Collider) continue;

// 				CollisionPoints points = a->Collider->TestCollision(a->transform, b->Collider, b->transform);

// 				if (points.HasCollision)
// 				{
// 					BOBO_INFO("***********************************COLLISION OCCURED************************************");
// 					collisions.emplace_back(Collision{ a, b, points });
// 				}
// 			}
// 		}
// 	}

// 	float CalculateDeltaTime()
// 	{
// 		// Get the current time
// 		m_CurrentTime = std::chrono::high_resolution_clock::now();

// 		// Calculate the time difference between the current and previous frames
// 		std::chrono::duration<float> deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(m_CurrentTime - m_PreviousTime);

// 		// Update the previous time for the next frame
// 		m_PreviousTime = m_CurrentTime;

// 		// Return the delta time in seconds
// 		return deltaTime.count();
// 	}
// };
