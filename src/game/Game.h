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
	const glm::vec3 m_tablePosition = glm::vec3{ 0, -1.5, -4.5 };
	const glm::vec3 m_ballScale = glm::vec3{ 0.2, 0.2, 0.2 };
	const glm::vec3 m_firstBallPos = glm::vec3{ m_tablePosition.x, m_tablePosition.y + 1, m_tablePosition.z + 3 };
	const float m_ballDistance = .42;

	void SetBallPos(GameObject* ball, float xOffset, float zOffset)
	{
		ball->GetComponent<Transform>()->position = m_firstBallPos;
		ball->GetComponent<Transform>()->position.x += xOffset;
		ball->GetComponent<Transform>()->position.z += zOffset;
		ball->GetComponent<Transform>()->scale = m_ballScale;
		ball->GetComponent<Transform>()->rotation.y = -glm::pi<float>()/2;
		ball->GetComponent<Transform>()->rotation.z = glm::pi<float>() / 2;
	}

	// setting up balls initial positions
	void BallsSetup()
	{
		// 1st row
		auto solidOneBall = new GameObject();
		solidOneBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("solid_1"));
		solidOneBall->GetComponent<Transform>()->position = m_firstBallPos;
		solidOneBall->GetComponent<Transform>()->scale = m_ballScale;
		// 2nd row
		auto StripedNineBall = new GameObject();
		StripedNineBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("striped_9"));
		SetBallPos(StripedNineBall, -0.5*m_ballDistance, -m_ballDistance);
		auto SolidTwoBall = new GameObject();
		SolidTwoBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("solid_2"));
		SetBallPos(SolidTwoBall, 0.5*m_ballDistance, -m_ballDistance);
		// 3rd row
		auto StripedTenBall = new GameObject();
		StripedTenBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("striped_10"));
		SetBallPos(StripedTenBall, -m_ballDistance, -2*m_ballDistance);
		auto SolidEightBall = new GameObject();
		SolidEightBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("8_ball"));
		SetBallPos(SolidEightBall, 0, -2*m_ballDistance);
		auto SolidThreeBall = new GameObject();
		SolidThreeBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("solid_3"));
		SetBallPos(SolidThreeBall, m_ballDistance, -2*m_ballDistance);
		// 4th row 
		auto StripedElevenBall = new GameObject();
		StripedElevenBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("striped_11"));
		SetBallPos(StripedElevenBall, -1.5*m_ballDistance, -3 * m_ballDistance);
		auto SolidSevenBall = new GameObject();
		SolidSevenBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("solid_7"));
		SetBallPos(SolidSevenBall, -0.5*m_ballDistance, -3 * m_ballDistance);
		auto StripedFourteenBall = new GameObject();
		StripedFourteenBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("striped_14"));
		SetBallPos(StripedFourteenBall, 0.5*m_ballDistance, -3 * m_ballDistance);
		auto SolidFourBall = new GameObject();
		SolidFourBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("solid_4"));
		SetBallPos(SolidFourBall, 1.5*m_ballDistance, -3 * m_ballDistance);
		// 5th row
		auto SolidFiveBall = new GameObject();
		SolidFiveBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("solid_5"));
		SetBallPos(SolidFiveBall, -2*m_ballDistance, -4 * m_ballDistance);
		auto StripedThirteenBall = new GameObject();
		StripedThirteenBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("striped_13"));
		SetBallPos(StripedThirteenBall, -1 * m_ballDistance, -4 * m_ballDistance);
		auto StripedFifteenBall = new GameObject();
		StripedFifteenBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("striped_15"));
		SetBallPos(StripedFifteenBall, 0, -4 * m_ballDistance);
		auto SolidSixBall = new GameObject();
		SolidSixBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("solid_6"));
		SetBallPos(SolidSixBall, m_ballDistance, -4 * m_ballDistance);
		auto StripedTwelveBall = new GameObject();
		StripedTwelveBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("striped_12"));
		SetBallPos(StripedTwelveBall, 2*m_ballDistance, -4 * m_ballDistance);
	}

	// Setting up the scene models
	void SceneSetup() 
	{
		// table creation
		auto table = new GameObject();
		table->AddComponent<Material>(ModelLoader::GetModel("Pool_Table"), TextureLoader::GetTexture("Table_Top"));
		table->GetComponent<Transform>()->position = m_tablePosition;

		// cue creation
		auto cue = new GameObject();
		cue->AddComponent<Material>(ModelLoader::GetModel("pool_cue"), TextureLoader::GetTexture("cue"));
		cue->GetComponent<Transform>()->position = m_firstBallPos;
		cue->GetComponent<Transform>()->position += glm::vec3{ 2, 1, 2 };
		cue->GetComponent<Transform>()->rotation = glm::vec3{-0.5, glm::pi<float>()/4, 0};
		cue->GetComponent<Transform>()->scale = glm::vec3{ 0.7, 0.7, 0.7 };

		// lamp creation
		auto lamp = new GameObject();
		lamp->AddComponent<Material>(ModelLoader::GetModel("lamp"), TextureLoader::GetTexture("lamp"));
		lamp->GetComponent<Transform>()->position = m_tablePosition;
		lamp->GetComponent<Transform>()->position += glm::vec3{ 0, 5, 0 };

		// bookshelves creation
		auto shelf1 = new GameObject();
		shelf1->AddComponent<Material>(ModelLoader::GetModel("book_shelf"), TextureLoader::GetTexture("wood"));
		shelf1->GetComponent<Transform>()->position = m_tablePosition;
		shelf1->GetComponent<Transform>()->position += glm::vec3{ -7, 3, -20 };
		shelf1->GetComponent<Transform>()->rotation = glm::vec3{ 0, -glm::pi<float>()/2, 0 };
		auto shelf2 = new GameObject();
		shelf2->AddComponent<Material>(ModelLoader::GetModel("book_shelf"), TextureLoader::GetTexture("wood"));
		shelf2->GetComponent<Transform>()->position = m_tablePosition;
		shelf2->GetComponent<Transform>()->position += glm::vec3{ 7, 3, -20 };
		shelf2->GetComponent<Transform>()->rotation = glm::vec3{ 0, -glm::pi<float>() / 2, 0 };

		// chairs creation
		auto chair1 = new GameObject();
		chair1->AddComponent<Material>(ModelLoader::GetModel("chair_1"));
		chair1->GetComponent<Transform>()->position = m_tablePosition;
		chair1->GetComponent<Transform>()->position += glm::vec3{ 7, 0, 0 };
		chair1->GetComponent<Transform>()->rotation = glm::vec3{ 0, glm::pi<float>() / 4, 0 };
		auto chair2 = new GameObject();
		chair2->AddComponent<Material>(ModelLoader::GetModel("chair_2"));
		chair2->GetComponent<Transform>()->position = m_tablePosition;
		chair2->GetComponent<Transform>()->position += glm::vec3{ -7, 0, 0 };
		chair2->GetComponent<Transform>()->rotation = glm::vec3{ 0, glm::pi<float>(), 0 };

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

		// Setting up initial scene
		SceneSetup();

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

		// Example of Creating a GameObject with a Parent GameObject
		auto childObject = new GameObject(*object);

		// Creating point lights
		auto pointlight = new GameObject();
		Pointlight pl;
		pl.baseColor = { 0.7f, 0.3f, 0.1f }; // RGB, each value goes up to 1.0
		pl.position = { -5.0f, -5.0f, 5.0f }; // X, Y, Z (Positive Z is closer to camera)
		pl.intensity = 250.0f;
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