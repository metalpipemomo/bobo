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

	// Constants for objects position / rotation / scale
	const glm::vec3 tablePosition = glm::vec3{ 0, -1, -4 };
	const glm::vec3 ballScale = glm::vec3{ 0.2, 0.2, 0.2 };
	const glm::vec3 firstBallPos = glm::vec3{ tablePosition.x, tablePosition.y + 1, tablePosition.z + 3 };
	const float ballDistance = .42;

	void setBallPos(GameObject* ball, float xOffset, float zOffset)
	{
		ball->GetComponent<Transform>()->position = firstBallPos;
		ball->GetComponent<Transform>()->position.x += xOffset;
		ball->GetComponent<Transform>()->position.z += zOffset;
		ball->GetComponent<Transform>()->scale = ballScale;
		ball->GetComponent<Transform>()->rotation.y = -glm::pi<float>()/2;
		ball->GetComponent<Transform>()->rotation.z = glm::pi<float>() / 2;
	}

	// setting up balls initial positions
	void BallsSetup()
	{
		// 1st row
		auto solidOneBall = new GameObject();
		solidOneBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("solid_1"));
		solidOneBall->GetComponent<Transform>()->position = firstBallPos;
		solidOneBall->GetComponent<Transform>()->scale = ballScale;
		// 2nd row
		auto StripedNineBall = new GameObject();
		StripedNineBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("striped_9"));
		setBallPos(StripedNineBall, -0.5*ballDistance, -ballDistance);
		auto SolidTwoBall = new GameObject();
		SolidTwoBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("solid_2"));
		setBallPos(SolidTwoBall, 0.5*ballDistance, -ballDistance);
		// 3rd row
		auto StripedTenBall = new GameObject();
		StripedTenBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("striped_10"));
		setBallPos(StripedTenBall, -ballDistance, -2*ballDistance);
		auto SolidEightBall = new GameObject();
		SolidEightBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("8_ball"));
		setBallPos(SolidEightBall, 0, -2*ballDistance);
		auto SolidThreeBall = new GameObject();
		SolidThreeBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("solid_3"));
		setBallPos(SolidThreeBall, ballDistance, -2*ballDistance);
		// 4th row 
		auto StripedElevenBall = new GameObject();
		StripedElevenBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("striped_11"));
		setBallPos(StripedElevenBall, -1.5*ballDistance, -3 * ballDistance);
		auto SolidSevenBall = new GameObject();
		SolidSevenBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("solid_7"));
		setBallPos(SolidSevenBall, -0.5*ballDistance, -3 * ballDistance);
		auto StripedFourteenBall = new GameObject();
		StripedFourteenBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("striped_14"));
		setBallPos(StripedFourteenBall, 0.5*ballDistance, -3 * ballDistance);
		auto SolidFourBall = new GameObject();
		SolidFourBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("solid_4"));
		setBallPos(SolidFourBall, 1.5*ballDistance, -3 * ballDistance);
		// 5th row
		auto SolidFiveBall = new GameObject();
		SolidFiveBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("solid_5"));
		setBallPos(SolidFiveBall, -2*ballDistance, -4 * ballDistance);
		auto StripedThirteenBall = new GameObject();
		StripedThirteenBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("striped_13"));
		setBallPos(StripedThirteenBall, -1 * ballDistance, -4 * ballDistance);
		auto StripedFifteenBall = new GameObject();
		StripedFifteenBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("striped_15"));
		setBallPos(StripedFifteenBall, 0, -4 * ballDistance);
		auto SolidSixBall = new GameObject();
		SolidSixBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("solid_6"));
		setBallPos(SolidSixBall, ballDistance, -4 * ballDistance);
		auto StripedTwelveBall = new GameObject();
		StripedTwelveBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("striped_12"));
		setBallPos(StripedTwelveBall, 2*ballDistance, -4 * ballDistance);
	}

	// Setting up the scene models
	void SceneSetup() 
	{
		Camera::SetPosition(glm::vec3{ 10, 160, 0 });
		Camera::SetTarget(glm::vec3{ 0, 0, 0 });

		// table creation
		auto table = new GameObject();
		table->AddComponent<Material>(ModelLoader::GetModel("Pool_Table"), TextureLoader::GetTexture("Table_Top"));
		table->GetComponent<Transform>()->position = tablePosition;

		// cue creation
		auto cue = new GameObject();
		cue->AddComponent<Material>(ModelLoader::GetModel("pool_cue"), TextureLoader::GetTexture("cue"));
		cue->GetComponent<Transform>()->position = firstBallPos;
		cue->GetComponent<Transform>()->position += glm::vec3{ 2, 1, 2 };
		cue->GetComponent<Transform>()->rotation = glm::vec3{-0.5, glm::pi<float>()/4, 0};
		cue->GetComponent<Transform>()->scale = glm::vec3{ 0.7, 0.7, 0.7 };

		// lamp creation
		auto lamp = new GameObject();
		lamp->AddComponent<Material>(ModelLoader::GetModel("lamp"));
		lamp->GetComponent<Transform>()->position = tablePosition;
		lamp->GetComponent<Transform>()->position += glm::vec3{ 0, 5, 0 };

		// bookshelves creation
		auto shelf1 = new GameObject();
		shelf1->AddComponent<Material>(ModelLoader::GetModel("book_shelf"), TextureLoader::GetTexture("wood"));
		shelf1->GetComponent<Transform>()->position = tablePosition;
		shelf1->GetComponent<Transform>()->position += glm::vec3{ -7, 3, -15 };
		shelf1->GetComponent<Transform>()->rotation = glm::vec3{ 0, -glm::pi<float>()/2, 0 };
		auto shelf2 = new GameObject();
		shelf2->AddComponent<Material>(ModelLoader::GetModel("book_shelf"), TextureLoader::GetTexture("wood"));
		shelf2->GetComponent<Transform>()->position = tablePosition;
		shelf2->GetComponent<Transform>()->position += glm::vec3{ 7, 3, -15 };
		shelf2->GetComponent<Transform>()->rotation = glm::vec3{ 0, -glm::pi<float>() / 2, 0 };

		// chairs creation
		auto chair1 = new GameObject();
		chair1->AddComponent<Material>(ModelLoader::GetModel("chair_1"));
		chair1->GetComponent<Transform>()->position = tablePosition;
		chair1->GetComponent<Transform>()->position += glm::vec3{ 6, 0, 0 };
		chair1->GetComponent<Transform>()->rotation = glm::vec3{ 0, glm::pi<float>() / 4, 0 };

		// setup initial balls position
		BallsSetup();
	}

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

		/* Example of Creating Game Object Begins */
		// Create GameObject
		auto object = new GameObject();
		
		// Add a material component and tell that material what model to use and what texture to use
		// models are all loaded in from the assets/Models directory upon game start and the same is done for textures from the assets/Textures directory
		object->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("solid_3"));
		/* End Example */

		/* Example of Adding a Component to a Game Object */
		// Get the 'Transform' component from GameObject; Game Objects by default will be created with a Transform component attatched unless 
		// an argument is passed to the GameObject constructor saying not to - new GameObject(false)
		auto transform = object->GetComponent<Transform>();

		// Add the component; In this case, FunnyMove needs two parameters: a transform reference & a speed at which the ball will rotate
		object->AddComponent<FunnyMove>(transform, 1);
		object->~GameObject();
		/* End Example */
		
		// Setting up initial scene
		SceneSetup();

		// Example of Creating a GameObject with a Parent GameObject
		auto childObject = new GameObject(*object);

		// Creating point lights
		auto pointlight = new GameObject();
		Pointlight pl;
		pl.baseColor = { 0.7f, 0.3f, 0.1f }; // RGB, each value goes up to 1.0
		pl.position = { -5.0f, -5.0f, 5.0f }; // X, Y, Z (Positive Z is closer to camera)
		pointlight->AddComponent<PointlightComponent>(pl);

		auto spotlight = new GameObject();
		Spotlight sl;
		sl.baseColor = { 0.7, 0.7, 0.7 }; // RGB, same as before
		sl.cutoffAngle = 45.0f; // The radius of the cone
		sl.direction = { 0.0f, -1.0f, 0.0f }; // Direction of the light
		sl.position = { 0.0f, 5.0f, 0.0f }; // X, Y, Z
		spotlight->AddComponent<SpotlightComponent>(sl);

		/*------ AUDIO ------*/

		// Audio files are loaded from the assets/Sounds directory, they must be .mp3
		// The files can be accessed through a string identifier, which corresponds to
		// the file name without extensions. Case doesn't matter
		Audio::PlaySound("boom");
		Audio::PlaySound("punch");

		/*------ COROUTINES ------*/
		// Should be all self-explanatory; Just some examples
		// Wait for Seconds
		float waitTime = 3;
		auto printSomething = [=]() { Log("Printed after {} seconds.", waitTime); };
		auto c = CoroutineScheduler::StartCoroutine<WaitForSeconds>(printSomething, waitTime);

		// Wait for another Coroutine
		auto printAfter = []() { Log("Printed after time print."); };
		CoroutineScheduler::StartCoroutine<WaitForCoroutine>(printAfter, c);

		// Wait for some Condition
		auto printAfterEvaluation = []() { Log("Printed after evaluation"); };
		auto evaluator = []() { return Time::RealTimeSinceStartup() > 10; };
		CoroutineScheduler::StartCoroutine<WaitUntil>(printAfterEvaluation, evaluator);
	}
};