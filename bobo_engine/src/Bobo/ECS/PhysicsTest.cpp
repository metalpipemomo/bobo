// #include "bobopch.h"
// #include "Vector3.h"
// /*
// 	Anyone viewing this code. Stop.

// 	This is just temporary test code for our physics engine.

// 	Chances are this wont work.

// 	- Johnny "Ya boy" Johnson

// */

// struct GameObject {
// 	Vector3 position;
// 	Vector3 velocity;
// 	Vector3 force;
// 	float mass;
// };

// class PhysicsWorld {
// private:
// 	PhysicsWorld();

// 	std::vector<GameObject*> m_PhysicsObjects;
// 	Vector3 m_gravity = Vector3(0, -9.81f, 0);

// public:
// 	PhysicsWorld()
// 	{
// 		GameObject test;
// 		test.position = Vector3();
// 		test.velocity = Vector3();
// 		test.force = Vector3();
// 		test.mass = 1;
// 		AddObject(&test);
// 	}
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

// 	void Update(float dt)
// 	{
// 		for (GameObject* obj : m_PhysicsObjects)
// 		{
// 			obj->force += m_gravity * obj->mass;

// 			obj->velocity += obj->force / obj->mass * dt;

// 			obj->position += obj->velocity * dt;

// 			obj->force = Vector3(0,0,0);

// 			BOBO_CORE_INFO("Object Position: Y: " + std::to_string(obj->position.getY()));
// 		}

// 	}
// };
