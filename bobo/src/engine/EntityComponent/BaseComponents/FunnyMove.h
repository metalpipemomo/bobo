#pragma once

#include "../Component.h"
#include "Transform.h"
#include "../../Coroutine/Waits/WaitForSeconds.h"
#include "../../Coroutine/Waits/WaitForCoroutine.h"

#define PI 3.14159265

class FunnyMove : public Component
{
public:
	double m_interval;
	//Coroutine* m_firstCheck;
	FunnyMove(Transform* inputTransform) : p_transformReference(inputTransform), m_interval(0.0) {};
	void Update() 
	{
		m_interval -= 0.0005;
		if (m_interval < 0)
			m_interval = 4.0;
		p_transformReference->position.x = sin(m_interval * 0.5 * PI);
		p_transformReference->position.y = cos(m_interval * 0.5 * PI);
		p_transformReference->rotation.x = sin(m_interval * 0.5 * PI);
	}
private:
	Transform* p_transformReference;
};