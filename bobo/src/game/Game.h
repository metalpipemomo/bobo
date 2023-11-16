#pragma once

#include "engine/Bobo.h"

#include "GameComponents/FunnyMove.h"

#include "../engine/GameState/GameStateManager.h"
#include "GameState/GameOverState.h"
#include "GameState/MainMenuState.h"
#include "GameState/PauseMenuState.h"
#include "GameState/InGameState.h"

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
		GameStateManager::InitWithGameStates(
			{ 
				{ GameStateLabel::GAME_OVER, new GameOverState() },
				{ GameStateLabel::IN_GAME, new InGameState() },
				{ GameStateLabel::MAIN_MENU, new MainMenuState() }},
			new PauseMenuState());
		GameStateManager::EnterGameState(GameStateLabel::MAIN_MENU);

		// Create Scene
		SceneManager::CreateScene("Scene1");

		// Create GameObject
		auto object = new GameObject();

		// Add Component 'Transform' to GameObject
		object->AddComponent<Transform>();

		// Get the 'Transform' component from GameObject
		auto transform = object->GetComponent<Transform>();
		object->AddComponent<Material>(ModelLoader::GetModel("Ball_1"), TextureLoader::GetTexture("1"));

		// Log initial transform position values
		Log("Initial position x: {}, y: {}, z: {}",
			transform->position.x, transform->position.y, transform->position.z);

		// Change value of x in transform position
		transform->position.x = 1;

		// Get transform again just to make sure it is properly being updated
		transform = object->GetComponent<Transform>();
		Log("New position x: {}", transform->position.x);
		object->AddComponent<FunnyMove>(transform, 1);

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