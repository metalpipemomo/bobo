#pragma once

#include "../../engine/EntityComponent/Component.h"
#include "../../engine/EntityComponent/BaseComponents/Transform.h"

#define PI 3.14159265

class RoofMove : public Component
{
public:
	RoofMove(Transform* transform) : p_Transform(transform), m_Speed(1.0), p_Scene(nullptr) {};
	RoofMove(Transform* transform, double speed, Scene* scene) : p_Transform(transform), m_Speed(speed),  p_Scene(scene)  {};
	void Update() 
	{
		m_Interval += m_Speed * Time::DeltaTime();
		p_Transform->position.y = m_Interval * 0.5 * PI;
		p_Transform->rotation.x = sin(m_Interval * 0.05 * PI)/2;
		p_Transform->rotation.y = sin(m_Interval * 0.05 * PI);

		if(p_Transform->position.y >= 80) {
			p_Scene->DestroyEntity(m_OwnerId);
		}
	}
private:
	Scene* p_Scene;
	Transform* p_Transform;
	double m_Speed;
	double m_Interval = 0;

};