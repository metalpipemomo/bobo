#include <Bobo.h>

class Game : public Bobo::Application
{
public:
	Game()
	{
		// Load all Models
		Bobo::ModelLoader::GetInstance()->LoadAllModels("src/Models");

		// Testing Audio
		Bobo::SoundInfo thwap = Bobo::SoundInfo("src/TestSFX/Punch.mp3", false, false);
		Bobo::AudioEngine::GetInstance()->LoadSound(thwap);
		Bobo::AudioEngine::GetInstance()->PlaySound(thwap);

		Bobo::SoundInfo boom = Bobo::SoundInfo("src/TestSFX/Boom.mp3", false, false);
		Bobo::AudioEngine::GetInstance()->LoadSound(boom);
		Bobo::AudioEngine::GetInstance()->PlaySound(boom);

		// Testing Scenes & ECS
		// Register Scene 1
		Bobo::Scene* s0 = new Bobo::Scene("Scene0");
		Bobo::SceneManager::GetInstance()->RegisterScene(s0);

		// Register Scene 2
		Bobo::Scene* s1 = new Bobo::Scene("Scene1");
		Bobo::SceneManager::GetInstance()->RegisterScene(s1);

		// Load the Scene at SceneIndex 0
		Bobo::SceneManager::GetInstance()->LoadScene(0);

		// Add Entity to Scene 1
		auto entity = s0->CreateEntity();

		// Add Transform Component to Entity
		s0->AddComponent<Bobo::Transform>(entity, glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1));

		// Create a Child Entity
		auto child = s0->CreateEntity(entity);

		// Destroy the child Entitiy
		s0->DestroyEntity(child);

		// Testing Coroutines
		auto cs = Bobo::CoroutineScheduler::GetInstance();

		auto callback = []()
			{
				Log("Coroutine 1 Callback Resolved (Using Unscaled Time, Should be ~2) - Real Time seconds {}",
					Bobo::Time::GetInstance()->RealTimeSinceStartup());
			};
		cs->StartCoroutine<Bobo::WaitForSeconds>(2, 1, callback);

		auto callback2 = []()
			{
				Log("Coroutine 2 Callback Resolved (Using Scaled Time, Should be ~3) - Real Time seconds {}",
					Bobo::Time::GetInstance()->RealTimeSinceStartup());
			};
		cs->StartCoroutine<Bobo::WaitForSeconds>(3, 0, callback2);

		auto callback3 = []()
			{
				Log("Coroutine 3 Callback Resolved (Over 10000 Frames Past - {}) - Real Time seconds {}",
					Bobo::Time::GetInstance()->FrameCount(), Bobo::Time::GetInstance()->RealTimeSinceStartup());
			};
		auto callback4 = []()
			{
				Log("Coroutine 4 Callback Resolved (Using Scaled Time, Should be ~3 Seconds past Callback 3 Resolve) - Real Time seconds {}",
					Bobo::Time::GetInstance()->RealTimeSinceStartup());
			};
		auto callback5 = []()
			{
				Log("Coroutine 5 Callback Resolved (Using Scaled Time, Should be ~6 Seconds past Callback 4 Resolve) - Real Time seconds {}",
					Bobo::Time::GetInstance()->RealTimeSinceStartup());
			};

		auto chainRoutine = new Bobo::WaitForEvaluation([]() { return Bobo::Time::GetInstance()->FrameCount() > 10000; }, callback3);
		chainRoutine->AddCoroutineToChain(new Bobo::WaitForSeconds(3, 0, callback4));
		chainRoutine->AddCoroutineToChain(new Bobo::WaitForSeconds(6, 0, callback5));
		cs->StartCoroutine(chainRoutine);

		auto callback6 = []()
			{
				Log("Coroutine 6 Callback Resolved (Waiting for Coroutine 3 to Resolve) - Real Time seconds {}",
					Bobo::Time::GetInstance()->RealTimeSinceStartup());
			};
		cs->StartCoroutine<Bobo::WaitForCoroutine>(chainRoutine->GetCoroutineInChain(0), callback6);
		auto callback7 = []()
			{
				Log("Coroutine 7 Callback Resolved (Waiting for Coroutine 5 to Resolve) - Real Time seconds {}",
					Bobo::Time::GetInstance()->RealTimeSinceStartup());
			};
		cs->StartCoroutine<Bobo::WaitForCoroutine>(chainRoutine->GetCoroutineInChain(2), callback7);

		auto evalFunc8 = []()
			{
				return Bobo::Time::GetInstance()->RealTimeSinceStartup() > 10;
			};
		auto callback8 = []()
			{
				Log("Coroutine 8 Callback Resolved (Over 10 Seconds have Past since Startup) - Real Time Seconds {}",
					Bobo::Time::GetInstance()->RealTimeSinceStartup());
			};
		cs->StartCoroutine<Bobo::WaitForEvaluation>(evalFunc8, callback8);
	}

	~Game()
	{

	}
};

Bobo::Application* Bobo::CreateApplication()
{
	return new Game();
};