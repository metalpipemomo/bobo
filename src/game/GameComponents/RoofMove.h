#pragma once

#include "../../engine/EntityComponent/Component.h"
#include "../../engine/EntityComponent/BaseComponents/Transform.h"

#define PI 3.14159265

class RoofMove : public Component
{
public:
	RoofMove(Transform* transform, double speed) : p_Transform(transform), m_Speed(speed) {};
	void Update() 
	{
		if(m_IsMoving) 
		{
			m_Interval += m_Speed * Time::DeltaTime();
			p_Transform->position.y += m_Interval * 0.5 * PI;
			p_Transform->rotation.x = sin(m_Interval * 0.05 * PI)/3;
			p_Transform->rotation.y = sin(m_Interval * 0.05 * PI);

			if(p_Transform->position.y >= 80) {
				SceneManager::GetActiveScene()->DestroyEntity(m_OwnerId);
			}
		}
	}
	void StartMove() 
	{
		m_IsMoving = true;
	}
private:
	Transform* p_Transform;
	double m_Speed;
	double m_Interval = 0;
	bool m_IsMoving = false;

};