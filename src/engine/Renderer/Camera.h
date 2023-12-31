#pragma once

#include "../../bpch.h"
#include "../../Game/GameComponents/ObjectTag.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Physics/Rigidbody.h"

class Camera
{
public:
	static void Init(float aspectRatio)
	{
		auto c = GetInstance();

		// Set all the members
		c->m_Position = glm::vec3{ 10, 0, 10 }; // Camera position
		c->m_Target = glm::vec3{ 0, 0, -1 }; // What Camera is looking at
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

	static void MoveUp(float speed) {
	
		auto c = GetInstance();
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 moveAxis = glm::vec3(0.0f, 1.0f, 0.0f);
		c->m_Position += speed * moveAxis;
		c->m_View = glm::lookAt(c->m_Position, c->m_Position + c->cameraFront, {0.0f, 1.0f, 0.0f});
	}

	static short SwitchMode() {
		auto c = GetInstance(); // Change the mode
		if (GameStateManager::GetCurrentState() != GameStateManager::FetchGameState(GameStateLabel::IN_GAME)) return c->m_mode;
		if (c->m_mode == 3) {
			SwitchLamp(); // Switch lamp back on since we're activating it
			if (c->m_firstMode) { // Remember past action to get into mode 3
				c->m_mode = 1;
				c->m_distance = 15.0f;
				return c->m_mode;
			}
		}
		if (Input::GetKey(GLFW_KEY_LEFT_CONTROL)) // Have to hold CTRL and TAB in order to get freecam
			c->m_mode = 0;
		else if (c->m_mode == 1 && GetTarget() && !(c->m_cueBallSunk)) { // Only switch to mode 2 if we found target
			c->m_mode = 2;
			c->m_distance = 5.0f;
		}
		else if (c->m_mode == 2 && Overcam()) {

		}
		else {
			c->m_mode = 1;
			c->m_distance = 15.0f;
		}
		return c->m_mode; // Return m-mode for all that cares about it
	}

	static bool Overcam() {
		auto c = GetInstance(); // Change the mode to above for ball placement
		if (c->m_mode == 3)
			return false;
		if (c->m_mode == 2) { // We record what our last mode was
			c->m_firstMode = true;
		}
		else {
			c->m_firstMode = false;
		}
		SwitchLamp(); // Switch the lamp so it's hidden
		c->m_mode = 3; // Set mode correctly
		SetCameraPositionAndLookAt({ 0.0,20.0,-4.4f }, { 0.0,0.0,-4.5f }); // Above view
		return true;
	}
	
	static void CueballSunk(bool inputBool) {
		auto c = GetInstance();
		if (c->m_cueBallSunk == inputBool)
			return;
		c->m_cueBallSunk = inputBool;
		if (c->m_mode == 2 && c->m_cueBallSunk) {
			SwitchMode();
		}
	}

	static short GetMode() {
		auto c = GetInstance(); // Return the mode number.
		return c->m_mode;
	}

	static void SwitchLamp() {
		auto scene = SceneManager::GetActiveScene();
		auto objects = scene->GetComponentsOfType<ObjectTag>();
		auto c = GetInstance();
		for (auto object : objects) { // Search thuogh all objects to find the tag of the cueBall
			if ((object->tag == "lamp")) // Look for lamp object
			{
				if (c->m_mode == 3) {
					((scene->GetComponent<Transform>(object->m_OwnerId)))->position = glm::vec3{ 0, -1.5, -4.5 }; // Current table view
					(scene->GetComponent<Transform>(object->m_OwnerId))->position += glm::vec3{ 0, 5, 0 };
				}
				else {
					((scene->GetComponent<Transform>(object->m_OwnerId)))->position = glm::vec3{ 0, -1.5, -4.5 }; // Current table view
					(scene->GetComponent<Transform>(object->m_OwnerId))->position += glm::vec3{ 0, 500, 0 }; // It's far in the sky now
				}
				return;
			}
		}
	}

	// handles movement of the camera while in freecam
	static void FreeCamControls() {
		if (Input::GetKey(GLFW_KEY_W) && Input::GetKey(GLFW_KEY_LEFT_CONTROL))
		{
			MoveUp(10.0f * Time::DeltaTime());
		}
		else if (Input::GetKey(GLFW_KEY_W))
		{
			MoveForward(10.0f * Time::DeltaTime());
		}
		if (Input::GetKey(GLFW_KEY_S) && Input::GetKey(GLFW_KEY_LEFT_CONTROL))
		{
			MoveUp(-10.0f * Time::DeltaTime());
		}
		else if (Input::GetKey(GLFW_KEY_S))
		{
			MoveForward(-10.0f * Time::DeltaTime());
		}

		if (Input::GetKey(GLFW_KEY_D))
		{
			MoveRight(10.0f * Time::DeltaTime());
		}
		if (Input::GetKey(GLFW_KEY_A))
		{
			MoveRight(-10.0f * Time::DeltaTime());
		}

		if (Input::GetKey(GLFW_KEY_UP)) {

			LookUp(50.0f * Time::DeltaTime());
		}

		if (Input::GetKey(GLFW_KEY_DOWN)) {

			LookUp(-50.0f * Time::DeltaTime());
		}

		if (Input::GetKey(GLFW_KEY_LEFT)) {

			LookRight(-50.0f * Time::DeltaTime());
		}

		if (Input::GetKey(GLFW_KEY_RIGHT)) {

			LookRight(50.0f * Time::DeltaTime());
		}
	}

	// handles movement of the camera when in slide view (essentially when focused on something)
	static void SlideControls() {
		auto c = GetInstance();
		if (Input::GetKey(GLFW_KEY_D))
		{
			MoveRight(c->m_distance * Time::DeltaTime()); // Scale left/right amount by distance away. Allows precision up close.
		}
		if (Input::GetKey(GLFW_KEY_A))
		{
			MoveRight(-c->m_distance * Time::DeltaTime()); // Scale left/right amount by distance away. Allows precision up close.
		}
		if (Input::GetKey(GLFW_KEY_W))
		{
			c->m_distance += -10.0f * Time::DeltaTime(); // Goes in.
		}
		if (Input::GetKey(GLFW_KEY_S))
		{
			c->m_distance += 10.0f * Time::DeltaTime(); // Goes out.
		}
	}

	// gets the camera target for the ball-targeting camera mode
	static bool GetTarget() {
		auto scene = SceneManager::GetActiveScene();
		auto objects = scene->GetComponentsOfType<ObjectTag>();
		auto c = GetInstance();
		for (auto object : objects) { // Search thuogh all objects to find the tag of the cueBall
			if ((object->tag == "cueBall" || object->tag == "cueBallGhost") && scene->GetComponent<Rigidbody>(object->m_OwnerId)->IsEnabled())
			{
				c->m_trackObjectCords = &(scene->GetComponent<Transform>(object->m_OwnerId))->position; // Get the position pointer.
				return true;
			}
		}
		return false;
	}

	static void SetCameraPositionAndLookAt(const glm::vec3 newCameraPos, const glm::vec3 targetPos) {
		auto c = GetInstance();
		c->m_Position = newCameraPos; // Set position
		c->cameraFront = glm::normalize(targetPos - c->m_Position); // Set to be towards the taret
		c->m_View = glm::lookAt(c->m_Position, c->m_Position + c->cameraFront, { 0.0f, 1.0f, 0.0f });
	}

	static void HoriSphereNormalizeDistance(const glm::vec3 targetObject) {
		auto c = GetInstance();
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		// Normalize based on radius on X and Z away from the object based on m_distance
		c->m_Position = targetObject + glm::normalize(glm::vec3( c->m_Position.x - targetObject.x,0.0,c->m_Position.z - targetObject.z ))* c->m_distance;
		c->m_Position.y += c->m_distance / 4.0f; // Create an angel looking down at the object, pushing up based on distance.
	}

	static void HandleCameraEvents() {
		auto c = GetInstance(); // Return the mode number.
		if (Input::GetKeyDown(GLFW_KEY_TAB) && !c->m_camLock) { // Use Key down so that we're not going through toggles at extreme speed
			SwitchMode(); // Switch mode of the camera.
		}
		if (!c->m_mode) { // If 0, essentially.
			FreeCamControls();
		}
		else if (c->m_mode == 1) { // If 1, table view
			SlideControls();
			if (c->m_distance < 10.0f) { // Clamp the distance values
				c->m_distance = 10.0f;
			}
			else if (c->m_distance > 20.0f) {
				c->m_distance = 20.0f;
			}
			SetCameraPositionAndLookAt(c->m_Position, { 0.0,0.0,-4.5f }); // We are assuming that the table won't move.
			HoriSphereNormalizeDistance({ 0.0,0.0,-4.5f });
		}
		else if (c->m_mode == 2) { // If 2, ball view
			SlideControls();
			if (c->m_distance < 2.0f) { // Clamp the distance values
				c->m_distance = 2.0f;
			}
			else if (c->m_distance > 10.0f) {
				c->m_distance = 10.0f;
			}
			SetCameraPositionAndLookAt(c->m_Position,(*(c->m_trackObjectCords)));
			HoriSphereNormalizeDistance((*(c->m_trackObjectCords)));
		}
	}

	static void MoveForward(float speed)
	{
		auto c = GetInstance();
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		c->m_Position += speed * c->cameraFront;
		c->m_View = glm::lookAt(c->m_Position, c->m_Position + c->cameraFront, {0.0f, 1.0f, 0.0f});
	}

	static void MoveRight(float speed)
	{
		auto c = GetInstance();
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 moveAxis = glm::normalize(glm::cross(c->cameraFront, {0.0f, 1.0f, 0.0f}));;
		c->m_Position += speed * moveAxis;
		c->m_View = glm::lookAt(c->m_Position, c->m_Position + c->cameraFront, {0.0f, 1.0f, 0.0f});
	}

	static void LookUp(float speed){
		auto c = GetInstance();
		c->m_Pitch += speed;

		// Clamp pitch to prevent flipping upside down
		if (c->m_Pitch > 89.0f) c->m_Pitch = 89.0f;
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		// Update camera front vector using yaw and pitch angles
		glm::vec3 front;
		front.x = cos(glm::radians(c->m_Yaw)) * cos(glm::radians(c->m_Pitch));
		front.y = sin(glm::radians(c->m_Pitch));
		front.z = sin(glm::radians(c->m_Yaw)) * cos(glm::radians(c->m_Pitch));
		c->cameraFront = glm::normalize(front);
		c->m_View = glm::lookAt(c->m_Position, c->m_Position + c->cameraFront,  {0.0f, 1.0f, 0.0f});
	}

	static void LookRight(float speed){
		auto c = GetInstance();
		c->m_Yaw += speed;

		// Clamp pitch to prevent flipping upside down
		if (c->m_Pitch > 89.0f) c->m_Pitch = 89.0f;
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		// Update camera front vector using yaw and pitch angles
		glm::vec3 front;
		front.x = cos(glm::radians(c->m_Yaw)) * cos(glm::radians(c->m_Pitch));
		front.y = sin(glm::radians(c->m_Pitch));
		front.z = sin(glm::radians(c->m_Yaw)) * cos(glm::radians(c->m_Pitch));
		c->cameraFront = glm::normalize(front);
		c->m_View = glm::lookAt(c->m_Position, c->m_Position + c->cameraFront, { 0.0f, 1.0f, 0.0f });
	}

	static void lockCam() {
		auto c = GetInstance();
		c->m_camLock = !c->m_camLock; // A quick lock of camera
	}

	static void Update()
	{
		auto c = GetInstance();
		c->cameraRight = { -glm::cos(c->m_Yaw), 0, glm::sin(c->m_Yaw) };
		c->cameraRight = glm::normalize(c->cameraRight);
		c->cameraUp = glm::cross(c->cameraFront, c->cameraRight);
		c->cameraUp = glm::normalize(c->cameraUp);
	}

	static glm::vec3 GetPosition() { return GetInstance()->m_Position; }
	static glm::vec3 GetCameraFront() { return GetInstance()->cameraFront; }
	static glm::vec3 GetCameraRight() { return GetInstance()->cameraRight; }
	static glm::vec3 GetCameraUp() { return GetInstance()->cameraUp; }
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
		m_View = glm::lookAt(m_Position, m_Position + m_Target, glm::vec3{ 0, 1, 0 });
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

	// mode //

	bool m_camLock = false;
	bool m_cueBallSunk = false;
	bool m_firstMode = 0; // Mode tracking last
	short m_mode = 1; // 0 = Freecam, 1 = Table, 2 = Ball
	glm::vec3* m_trackObjectCords; // Coordinates pointer of what we're tracking.
	float m_distance = 10.0f; // Distance from orbit.

	// johnny variables//

	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraRight;
	glm::vec3 cameraUp;
	float m_Yaw   = -90.0f; // Initialize yaw angle
	float m_Pitch = 0.0f;   // Initialize pitch angle
	///

	glm::mat4 m_View;
	glm::mat4 m_Projection;
};