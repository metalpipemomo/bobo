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
	bool enabled;

	CueBallGhost(Rigidbody *body, Rigidbody* cbRb) {
		rb = body;
		cueball = cbRb;
		resetPos = cbRb->GetPosition();

		// if this is colliding with something, add it to the list of collision buddies
		rb->SetOnCollision([this](BodyID other) {
			this->collisions.push_back(other);
			auto pos = this->rb->GetTransform()->position;
			});

		// if it stops colliding with something, remove it from the list
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
		// if this isn't enabled, hide it forever
		if (!enabled)
		{
			rb->SetPositionHard(Vec3(resetPos.GetX(), resetPos.GetY() - 2.0f, resetPos.GetZ()));
			return;
		}

		// if the ball wasn't enabled, but now is, hide the cueball and activate the ghost ball in the right position
		if (!wasEnabled) {
			Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterface().ActivateBody(rb->GetBodyID());
			cueball->SetVelocity({ 0,0,0 });
			rb->SetPositionHard(resetPos);
			wasEnabled = true;
		}

		float xMove = 0;
		float zMove = 0;

		// get the movements of the ball
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

		// force the new, moved x position to be within bounds
		xMove += pos.GetX();
		xMove = max(xMove, -3.9f);
		xMove = min(xMove, 3.9f);

		// force the new, moved z position to be within bounds
		zMove += pos.GetZ();
		zMove = min(zMove, 3.3f);
		zMove = max(zMove, -12.3f);

		// set the position of the ball to the new position
		rb->SetPositionHard(Vec3(xMove, -1.05f, zMove));

		// if the player tries to place the ball, and the ball is only colliding with the
		// play area of the table and NOTHING ELSE (no balls, no walls), then place the
		// cue ball and disable the ghost ball
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
		// put the camera in the overhead mode
		Camera::lockCam();
		Camera::Overcam();
		enabled = true;
		Camera::GetTarget();
	}

	void Disable() {
		enabled = false;
		wasEnabled = false;
		// reset the camera to not be in the overhead mode
		if (m_preset) {
			Camera::CueballSunk(false);
			Camera::lockCam();
			Camera::SwitchMode(); // Switch back to last camera
		}
		m_preset = true;
		Physics::GetInstance()->GetPhysicsSystem()->GetBodyInterface().DeactivateBody(rb->GetBodyID());
		Camera::GetTarget();
	}

private:
	bool wasEnabled;
	bool m_preset = false; // Ignore first deactivation for camera
};