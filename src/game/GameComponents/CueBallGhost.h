#pragma once

#include "../../engine/EntityComponent/Component.h"
#include "../../engine/EntityComponent/BaseComponents/Transform.h"
#include "../../engine/Input.h"
#include <GLFW/glfw3.h>
#include <math.h>

class CueBallGhost : public Component
{
public:
	Rigidbody* rb;
	std::vector<BodyID> collisions;
	Rigidbody* cueball;
	Vec3 resetPos;

	CueBallGhost(Rigidbody *body, Rigidbody* cbRb) {
		rb = body;
		cueball = cbRb;
		resetPos = cbRb->GetPosition();

		rb->SetOnCollision([this](BodyID other) {
			this->collisions.push_back(other);
			auto pos = this->rb->GetTransform()->position;
			});

		rb->SetOnCollisionEnd([this](BodyID other) {
			auto it = find(this->collisions.begin(), this->collisions.end(), other);
			if (it != this->collisions.end())
			{
				this->collisions.erase(it);
			}
			});
	}

	void Awake() {
		Disable();
	}

	void Update() {
		if (!enabled)
		{
			rb->SetPositionHard(Vec3(resetPos.GetX(), resetPos.GetY() - 2.0f, resetPos.GetZ()));
			return;
		}

		if (!wasEnabled) {
			Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterface().ActivateBody(rb->GetBodyID());
			cueball->SetVelocity({ 0,0,0 });
			rb->SetPositionHard(resetPos);
			wasEnabled = true;
		}

		float xMove = 0;
		float zMove = 0;

		if (Input::GetKey(GLFW_KEY_W))
		{
			zMove -= 0.03f;
		}
		if (Input::GetKey(GLFW_KEY_S))
		{
			zMove += 0.03f;
		}
		if (Input::GetKey(GLFW_KEY_A))
		{
			xMove -= 0.03f;
		}
		if (Input::GetKey(GLFW_KEY_D))
		{
			xMove += 0.03f;
		}

		auto pos = rb->GetPosition();

		xMove += pos.GetX();
		xMove = max(xMove, -3.9f);
		xMove = min(xMove, 3.9f);

		zMove += pos.GetZ();
		zMove = min(zMove, 3.3f);
		zMove = max(zMove, -12.3f);

		rb->SetPositionHard(Vec3(xMove, -1.05f, zMove));

		if (Input::GetKey(GLFW_KEY_P) && collisions.size() < 2) 
		{
			cueball->SetPositionHard(rb->GetPosition());
			cueball->EnableBody();
			Disable();
			
		}
	}

	void Enable() {
		if (enabled) // Ignore repeated enable commands
			return;
		Camera::lockCam();
		Camera::Overcam();
		enabled = true;
		Camera::GetTarget();
	}

	void Disable() {
		enabled = false;
		wasEnabled = false;
		if (m_preset) {
			Camera::CueballSunk(false);
			Camera::lockCam();
			Camera::SwitchMode(); // Switch back to last camera
		}
		m_preset = true;
		Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterface().DeactivateBody(rb->GetBodyID());
		//SceneManager::GetActiveScene()->GetComponent<Renderer>(m_OwnerId);
		Camera::GetTarget();
	}

private:
	bool enabled;
	bool wasEnabled;
	bool m_preset = false; // Ignore first deactivation for camera
};