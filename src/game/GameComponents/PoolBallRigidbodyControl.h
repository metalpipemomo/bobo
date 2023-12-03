#pragma once

#include "../../engine/EntityComponent/Component.h"
#include "../../engine/EntityComponent/BaseComponents/Transform.h"
#include "../../engine/Physics/Rigidbody.h"


class PoolBallRigidbodyControl : public Component
{
public:
	PoolBallRigidbodyControl(Rigidbody* rigidbody, double speed) : p_Rigidbody(rigidbody), m_MinimumSpeed(speed) {};
	void Update() 
	{	
		if(p_Rigidbody != nullptr) 
		{
			auto temp = (p_Rigidbody->GetVelocity());

			if((abs(temp.x) + abs(temp.y) + abs(temp.z)) > m_MinimumSpeed)
			{
				m_TimeInMotion += Time::DeltaTime();
				if(m_TimeInMotion >= m_MaxTimeInMotion) 
				{
					p_Rigidbody->AddLinearVelocity(JPH::Vec3(-temp.x, -temp.y, -temp.z));
				}
			} else {
				m_TimeInMotion = 0;
			}
		
		}
	}
private:
	Rigidbody* p_Rigidbody;
	double m_MinimumSpeed;
	float m_TimeInMotion = 0;
	float m_MaxTimeInMotion = 5; 
};