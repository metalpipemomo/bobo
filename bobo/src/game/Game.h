#pragma once

#include "engine/Bobo.h"

class Game
{
public:
	Game()
	{
		Setup();
	}

private:
	void Setup()
	{
		// Create Scene
		auto sm = SceneManager::GetInstance();
		sm->CreateScene("Scene1");

		// Create GameObject
		auto object = new GameObject();

		// Add Component 'Transform' to GameObject
		object->AddComponent<Transform>();

		// Get the 'Transform' component from GameObject
		auto transform = object->GetComponent<Transform>();

		// Log initial transform position values
		Log("Initial position x: {}, y: {}, z: {}",
			transform->position.x, transform->position.y, transform->position.z);

		// Change value of x in transform position
		transform->position.x = 5;

		// Get transform again just to make sure it is properly being updated
		transform = object->GetComponent<Transform>();
		Log("New position x: {}", transform->position.x);


	}
};