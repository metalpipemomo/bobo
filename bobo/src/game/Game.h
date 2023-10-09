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
		/*------ BASIC SETUP ------*/

		// Create Scene
		SceneManager::CreateScene("Scene1");

		// Create GameObject
		auto object = new GameObject();

		// Add Component 'Transform' to GameObject
		object->AddComponent<Transform>();

		// Get the 'Transform' component from GameObject
		auto transform = object->GetComponent<Transform>();
		object->AddComponent<Material>(ModelLoader::GetModel("cube"), TextureLoader::GetTexture("crate"));

		// Log initial transform position values
		Log("Initial position x: {}, y: {}, z: {}",
			transform->position.x, transform->position.y, transform->position.z);

		// Change value of x in transform position
		transform->position.x = 5;

		// Get transform again just to make sure it is properly being updated
		transform = object->GetComponent<Transform>();
		Log("New position x: {}", transform->position.x);

		// Creating a GameObject with a Parent GameObject
		auto childObject = new GameObject(*object);

		/*------ AUDIO ------*/

		// Audio files are loaded from the src/game/Sounds directory, they must be mp3
		// The files can be accessed through a string identifier, which corresponds to
		// the file name, all lowercase without extensions
		Audio::PlaySound("boom");
		Audio::PlaySound("punch");

		/*------ COROUTINES ------*/

		// I hope this is all self-explanatory
		float waitTime = 3;
		auto printSomething = [=]() { Log("Printed after {} seconds.", waitTime); };
		auto printAfter = []() { Log("Printed after time print."); };
		auto printAfterEvaluation = []() { Log("Printed after evaluation"); };
		auto evaluator = []() { return Time::RealTimeSinceStartup() > 10; };

		auto c = CoroutineScheduler::StartCoroutine<WaitForSeconds>(printSomething, waitTime);
		CoroutineScheduler::StartCoroutine<WaitForCoroutine>(printAfter, c);
		CoroutineScheduler::StartCoroutine<WaitUntil>(printAfterEvaluation, evaluator);

	}
};