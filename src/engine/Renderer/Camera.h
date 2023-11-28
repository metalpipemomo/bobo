#pragma once

#include "../../bpch.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	static void Init(float aspectRatio)
	{
		auto c = GetInstance();

		// Set all the members
		c->m_Position = glm::vec3{ 0, 0, 10 }; // Camera position
		c->m_Target = glm::vec3{ 0, 0, 1 }; // What Camera is looking at
		c->m_AspectRatio = aspectRatio;
		c->m_NearClipping = 1.0f;
		c->m_FarClipping = 1000.0f;

		c->UpdateViewMatrix();
		c->UpdateProjectionMatrix();

		BOBO_INFO("Camera initialized!");
	}

	static void SetPosition(glm::vec3 position)
	{
		auto c = GetInstance();
		c->m_Position = position;
		c->UpdateViewMatrix();
	}


	static void SetTarget(glm::vec3 target)
	{
		auto c = GetInstance();
		c->m_Target = target;
		c->UpdateViewMatrix();
	}

	static void SetAspectRatio(float aspectRatio)
	{
		auto c = GetInstance();
		c->m_AspectRatio = aspectRatio;
		c->UpdateProjectionMatrix();
	}

	static glm::vec3 MoveTowards(glm::vec3 currentPosition, glm::vec3 targetPosition, float maxDistance)
	{
		glm::vec3 direction = targetPosition - currentPosition;
		float distance = glm::length(direction);
		if (distance <= maxDistance || distance == 0.0f)
		{
			return currentPosition;
		}

		glm::vec3 result = currentPosition + (glm::normalize(direction) * maxDistance);
		return result;
	}

	static void MoveForward(float speed)
	{
		auto c = GetInstance();
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		c->m_Position += speed * cameraFront;
		c->m_View = glm::lookAt(c->m_Position, c->m_Position + cameraFront, {0.0f, 1.0f, 0.0f});
	}

	static void MoveRight(float speed)
	{
		auto c = GetInstance();
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 moveAxis = glm::vec3(-1,0,0);
		c->m_Position += speed * moveAxis;
		c->m_View = glm::lookAt(c->m_Position, c->m_Position + cameraFront, {0.0f, 1.0f, 0.0f});
	}

	static void LookUp(float speed){
		//auto c = GetInstance();
		//glm::vec3 currentTarget = c->m_Target; // Example vector
		//float angle = glm::radians(10.0f); // Rotate 90 degrees
		//glm::vec3 rotationAxis = glm::vec3(0,1,0); // Rotate around Z axis
		//glm::quat quaternion = glm::angleAxis(angle, rotationAxis);
		//glm::vec3 rotatedVector = quaternion * currentTarget;
		//c->m_Target = rotatedVector;

		//BOBO_INFO(c->m_Target.x);
		//BOBO_INFO(c->m_Target.y);
		//BOBO_INFO(c->m_Target.z);
		//BOBO_INFO("------");
		/*c->UpdateViewMatrix();*/
	}

	static glm::vec3 GetPosition() { return GetInstance()->m_Position; }
	static glm::mat4 GetViewMatrix() { return GetInstance()->m_View; }
	static glm::mat4 GetProjectionMatrix() { return GetInstance()->m_Projection; }

private:
	static Camera* GetInstance()
	{
		static Camera* instance = new Camera();
		return instance;
	}

	Camera() {}

	void UpdateViewMatrix()
	{
		m_View = glm::lookAt(m_Position, m_Target, glm::vec3{ 0, 1, 0 });
	}

	void UpdateProjectionMatrix()
	{
		m_Projection = glm::perspective(glm::radians(45.0f), m_AspectRatio, m_NearClipping, m_FarClipping);
	}

	glm::vec3 m_Position;
	glm::vec3 m_Target;
	
	float m_Fov;
	float m_AspectRatio;
	float m_NearClipping;
	float m_FarClipping;

	glm::mat4 m_View;
	glm::mat4 m_Projection;
};