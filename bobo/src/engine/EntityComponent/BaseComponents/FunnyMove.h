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
	FunnyMove(Transform* inputTransform) : p_transformReference(inputTransform), m_interval(0.0)
	{
		FunnyMove* selfReference = this;
		auto updateSelf = [this]() { this->Update();};
		//m_firstCheck = CoroutineScheduler::StartCoroutine<WaitForSeconds>(updateSelf,0.1);
		CoroutineScheduler::StartCoroutine<WaitForSeconds>(updateSelf, 0.1,TimeMode::DeltaTime,true);
	};
	void Update() 
	{
		m_interval -= 0.0005;
		if (m_interval < 0)
			m_interval = 4.0;
		p_transformReference->position.x = sin(m_interval * 0.5 * PI);
		p_transformReference->position.y = cos(m_interval * 0.5 * PI);
		p_transformReference->rotation.x = sin(m_interval * 0.5 * PI);
		auto updateSelf = [this]() { this->Update(); };
		//m_firstCheck = CoroutineScheduler::StartCoroutine<WaitForCoroutine>(updateSelf,m_firstCheck);
	}
private:
	Transform* p_transformReference;
};