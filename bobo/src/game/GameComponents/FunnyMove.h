#pragma once

#include "../../engine/EntityComponent/Component.h"
#include "../../engine/EntityComponent/BaseComponents/Transform.h"

#define PI 3.14159265

class FunnyMove : public Component
{
public:
	FunnyMove(Transform* transform) : p_Transform(transform), m_Speed(1.0) {};
	FunnyMove(Transform* transform, double speed) : p_Transform(transform), m_Speed(speed) {};
	void Update() 
	{
		m_Interval -= m_Speed * Time::DeltaTime();
		if (m_Interval < 0)
			m_Interval = 4.0;
		p_Transform->position.x = sin(m_Interval * 0.5 * PI);
		p_Transform->position.y = cos(m_Interval * 0.5 * PI);
		p_Transform->rotation.x = sin(m_Interval * 0.5 * PI);
	}
private:
	Transform* p_Transform;
	double m_Speed;
	double m_Interval = 0;
};