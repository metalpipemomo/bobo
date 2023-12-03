#pragma once

#include "engine/Bobo.h"

#include "../engine/Physics/Rigidbody.h"
#include "GameComponents/PoolBallRigidbodyControl.h"

#include "GameComponents/FunnyMove.h"

#include "../engine/GameState/GameStateManager.h"
#include "GameState/GameOverState.h"
#include "GameState/MainMenuState.h"
#include "GameState/PauseMenuState.h"
#include "GameState/InGameState.h"

#include "../engine/Notifications/NotificationManager.h"

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
	const glm::vec3 m_ballScale = glm::vec3{ 0.23, 0.23, 0.23 };
	const glm::vec3 m_firstBallPos = glm::vec3{ m_tablePosition.x, m_tablePosition.y+.5f, m_tablePosition.z -1};
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
		float minimumSpeed = .165f;
		Ref<SphereShape> s = new SphereShape(0.23);
		s->SetDensity(5000);

		auto solidOneBall = new GameObject();
		solidOneBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("solid_1"));
		solidOneBall->GetComponent<Transform>()->position = m_firstBallPos;
		solidOneBall->GetComponent<Transform>()->scale = m_ballScale;
		addRigidBodyToBall(solidOneBall, s);
		addPoolBallComponent(solidOneBall, minimumSpeed);
	

		// 2nd row
		auto StripedNineBall = new GameObject();
		StripedNineBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("striped_9"));
		SetBallPos(StripedNineBall, -0.5*m_ballDistance, -m_ballDistance);
		addRigidBodyToBall(StripedNineBall, s);
		addPoolBallComponent(StripedNineBall, minimumSpeed);
	
		//
		auto SolidTwoBall = new GameObject();
		SolidTwoBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("solid_2"));
		SetBallPos(SolidTwoBall, 0.5*m_ballDistance, -m_ballDistance);
		addRigidBodyToBall(SolidTwoBall, s);
		addPoolBallComponent(SolidTwoBall, minimumSpeed);
		// 3rd row
		auto StripedTenBall = new GameObject();
		StripedTenBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("striped_10"));
		SetBallPos(StripedTenBall, -m_ballDistance, -2*m_ballDistance);
		addRigidBodyToBall(StripedTenBall, s);
		addPoolBallComponent(StripedTenBall, minimumSpeed);
		//
		auto SolidEightBall = new GameObject();
		SolidEightBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("8_ball"));
		SetBallPos(SolidEightBall, 0, -2*m_ballDistance);
		addRigidBodyToBall(SolidEightBall, s);
		addPoolBallComponent(SolidEightBall, minimumSpeed);
		//
		auto SolidThreeBall = new GameObject();
		SolidThreeBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("solid_3"));
		SetBallPos(SolidThreeBall, m_ballDistance, -2*m_ballDistance);
		addRigidBodyToBall(SolidThreeBall, s);
		addPoolBallComponent(SolidThreeBall, minimumSpeed);
		// 4th row 
		auto StripedElevenBall = new GameObject();
		StripedElevenBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("striped_11"));
		SetBallPos(StripedElevenBall, -1.5*m_ballDistance, -3 * m_ballDistance);
		addRigidBodyToBall(StripedElevenBall, s);
		addPoolBallComponent(StripedElevenBall, minimumSpeed);
		//
		auto SolidSevenBall = new GameObject();
		SolidSevenBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("solid_7"));
		SetBallPos(SolidSevenBall, -0.5*m_ballDistance, -3 * m_ballDistance);
		addRigidBodyToBall(SolidSevenBall, s);
		addPoolBallComponent(SolidSevenBall, minimumSpeed);
		//
		auto StripedFourteenBall = new GameObject();
		StripedFourteenBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("striped_14"));
		SetBallPos(StripedFourteenBall, 0.5*m_ballDistance, -3 * m_ballDistance);
		addRigidBodyToBall(StripedFourteenBall, s);
		addPoolBallComponent(StripedFourteenBall, minimumSpeed);
		//
		auto SolidFourBall = new GameObject();
		SolidFourBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("solid_4"));
		SetBallPos(SolidFourBall, 1.5*m_ballDistance, -3 * m_ballDistance);
		addRigidBodyToBall(SolidFourBall, s);
		addPoolBallComponent(SolidFourBall, minimumSpeed);
		// 5th row
		auto SolidFiveBall = new GameObject();
		SolidFiveBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("solid_5"));
		SetBallPos(SolidFiveBall, -2*m_ballDistance, -4 * m_ballDistance);
		addRigidBodyToBall(SolidFiveBall, s);
		addPoolBallComponent(SolidFiveBall, minimumSpeed);
		//
		auto StripedThirteenBall = new GameObject();
		StripedThirteenBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("striped_13"));
		SetBallPos(StripedThirteenBall, -1 * m_ballDistance, -4 * m_ballDistance);
		addRigidBodyToBall(StripedThirteenBall, s);
		addPoolBallComponent(StripedThirteenBall, minimumSpeed);
		//
		auto StripedFifteenBall = new GameObject();
		StripedFifteenBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("striped_15"));
		SetBallPos(StripedFifteenBall, 0, -4 * m_ballDistance);
		addRigidBodyToBall(StripedFifteenBall, s);
		addPoolBallComponent(StripedFifteenBall, minimumSpeed);
		//
		auto SolidSixBall = new GameObject();
		SolidSixBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("solid_6"));
		SetBallPos(SolidSixBall, m_ballDistance, -4 * m_ballDistance);
		addRigidBodyToBall(SolidSixBall, s);
		addPoolBallComponent(SolidSixBall, minimumSpeed);
		//
		auto StripedTwelveBall = new GameObject();
		StripedTwelveBall->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("striped_12"));
		SetBallPos(StripedTwelveBall, 2*m_ballDistance, -4 * m_ballDistance);
		addRigidBodyToBall(StripedTwelveBall, s);
		addPoolBallComponent(StripedTwelveBall, minimumSpeed);

		auto cueball = new GameObject();
		cueball->AddComponent<Material>(ModelLoader::GetModel("ball"), TextureLoader::GetTexture("white"));
		cueball->GetComponent<Transform>()->position = m_firstBallPos + glm::vec3(0,0,7);
		cueball->GetComponent<Transform>()->scale = glm::vec3(.23,.23,.23);
		Ref<SphereShape> s2 = new SphereShape(0.23);
		s2->SetDensity(5000);
		addRigidBodyToBall(cueball, s2);
		cueball->GetComponent<Rigidbody>()->AddLinearVelocity(JPH::Vec3(0,0,-80));
	}

	void addRigidBodyToBall(GameObject *ball, Ref<SphereShape> s) 
	{
		auto transform = ball->GetComponent<Transform>();
		ball->AddComponent<Rigidbody>(s, transform->position, Quat::sIdentity(), EMotionType::Dynamic, Layers::MOVING);
		ball->GetComponent<Rigidbody>()->SetTransform(transform);
		ball->GetComponent<Rigidbody>()->SetBounce(0.4);
		ball->GetComponent<Rigidbody>()->SetFriction(.3);
	}

	void addPoolBallComponent(GameObject *ball, float minimumSpeed) 
	{
		//ball->AddComponent<PoolBallRigidbodyControl>(ball->GetComponent<Rigidbody>(), minimumSpeed);
	}

	void TableRigidBodySetUp() 
	{
		// table creation
		auto table = new GameObject();
		table->AddComponent<Material>(ModelLoader::GetModel("Pool_Table"), TextureLoader::GetTexture("Table_Top"));
		auto tableTransform = table->GetComponent<Transform>();
		tableTransform->position = m_tablePosition;
		table->AddComponent<Rigidbody>(new BoxShape(RVec3(4.2,.25,6.5)), tableTransform->position, Quat::sIdentity(), EMotionType::Static, Layers::NON_MOVING);
		table->GetComponent<Rigidbody>()->SetFriction(1);


		auto leftwall = new GameObject();
		leftwall->AddComponent<Material>(ModelLoader::GetModel("cube"), TextureLoader::GetTexture("Table_Top"));
		auto leftwallTransform = leftwall->GetComponent<Transform>();
		leftwallTransform->position =  glm::vec3{ -5, 1, -2.5 };
		leftwall->AddComponent<Rigidbody>(new BoxShape(RVec3(1,10,10)), leftwallTransform->position, Quat::sIdentity(), EMotionType::Static, Layers::NON_MOVING);
		///
		auto rightwall = new GameObject();
		rightwall->AddComponent<Material>(ModelLoader::GetModel("cube"), TextureLoader::GetTexture("Table_Top"));
		auto rightwallTransform = rightwall->GetComponent<Transform>();
		rightwallTransform->position = glm::vec3{ 5, 1, -2.5};
		rightwall->AddComponent<Rigidbody>(new BoxShape(RVec3(1,10,10)), rightwallTransform->position, Quat::sIdentity(), EMotionType::Static, Layers::NON_MOVING);
		///
		auto frontwall = new GameObject();
		frontwall->AddComponent<Material>(ModelLoader::GetModel("cube"), TextureLoader::GetTexture("Table_Top"));
		auto frontwallTransform = frontwall->GetComponent<Transform>();
		frontwallTransform->position = glm::vec3{ 0, 1, 2.5 };
		frontwall->AddComponent<Rigidbody>(new BoxShape(RVec3(10,10,1)), frontwallTransform->position, Quat::sIdentity(), EMotionType::Static, Layers::NON_MOVING);
		///
		auto backwall = new GameObject();
		backwall->AddComponent<Material>(ModelLoader::GetModel("cube"), TextureLoader::GetTexture("Table_Top"));
		auto backwallTransform = backwall->GetComponent<Transform>();
		backwallTransform->position = glm::vec3{ 0, 1, -11.5 };
		backwall->AddComponent<Rigidbody>(new BoxShape(RVec3(10,10,1)), backwallTransform->position, Quat::sIdentity(), EMotionType::Static, Layers::NON_MOVING);
		
		auto triggerBox = new GameObject();
		triggerBox->AddComponent<Material>(ModelLoader::GetModel("cube"), TextureLoader::GetTexture("8_ball"));
		triggerBox->GetComponent<Transform>()->scale = glm::vec3{2,2,2};
		triggerBox->GetComponent<Transform>()->position = m_firstBallPos + glm::vec3(0,20,-.5);
		triggerBox->AddComponent<Rigidbody>(new BoxShape(Vec3::sReplicate(2.0f)), 
			triggerBox->GetComponent<Transform>()->position, Quat::sIdentity(), EMotionType::Static, Layers::NON_MOVING, nullptr, true);
		auto triggerBoxRb = triggerBox->GetComponent<Rigidbody>();
		triggerBoxRb->SetTransform(triggerBox->GetComponent<Transform>());
		triggerBoxRb->SetOnCollision([](JPH::BodyID other) {
				Entity en = Physics::GetInstance()->GetEntityFromJoltRb(other);
				NotificationManager::SendNotification("The Ball has Fallen through the Box", NotificationTextColor::GREEN);
			});
	}

	// Setting up the scene models
	void SceneSetup() 
	{
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

		// setup initial balls position and rigidbody
		BallsSetup();
		TableRigidBodySetUp();
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