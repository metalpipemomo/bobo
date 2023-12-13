#pragma once

#include "../../../engine/EntityComponent/Component.h"
#include "../../../engine/EntityComponent/BaseComponents/Transform.h"

#define PI 3.14159265

class DestroyAfterFall : public Component
{
public:
	DestroyAfterFall(Transform* transform) : p_Transform(transform) {};
	void Update() 
	{
		// if the object has fallen past a certain point, destroy it
		if(p_Transform->position.y <= -40) {
				SceneManager::GetActiveScene()->DestroyEntity(m_OwnerId);
		}
		
	}

private:
	Transform* p_Transform;

};