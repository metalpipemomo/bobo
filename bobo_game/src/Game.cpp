#include <Bobo.h>

class Game : public Bobo::Application
{
public:
	Game()
	{
		// Load all Models
		Bobo::ModelLoader::GetInstance().LoadAllModels("src/Models");

		// Testing Audio
		Bobo::SoundInfo thwap = Bobo::SoundInfo("src/TestSFX/Punch.mp3", false, false);
		Bobo::AudioEngine::GetInstance().LoadSound(thwap);
		Bobo::AudioEngine::GetInstance().PlaySound(thwap);

		Bobo::SoundInfo boom = Bobo::SoundInfo("src/TestSFX/Boom.mp3", false, false);
		Bobo::AudioEngine::GetInstance().LoadSound(boom);
		Bobo::AudioEngine::GetInstance().PlaySound(boom);

		// Testing Scenes & ECS
		// Register Scene 1
		Bobo::Scene* s0 = new Bobo::Scene("Scene0");
		Bobo::SceneManager::GetInstance().RegisterScene(s0);

		// Register Scene 2
		Bobo::Scene* s1 = new Bobo::Scene("Scene1");
		Bobo::SceneManager::GetInstance().RegisterScene(s1);

		// Load the Scene at SceneIndex 0
		Bobo::SceneManager::GetInstance().LoadScene(0);

		// Add Entity to Scene 1
		auto entity = s0->CreateEntity();

		// Add Transform Component to Entity
		s0->AddComponent<Bobo::Transform>(entity, glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1));

		// Create a Child Entity
		auto child = s0->CreateEntity(entity);

		// Destroy the child Entitiy
		s0->DestroyEntity(child);

		// Coroutines
		auto cs = Bobo::CoroutineScheduler::GetInstance();
		float waitTime = 2.0f;
		auto callback = [&waitTime]()
			{
				Log("Coroutine ran after {} seconds!", waitTime);
			};
		cs.StartCoroutine<Bobo::WaitForSeconds>(waitTime, callback);

	}

	~Game()
	{

	}
};

Bobo::Application* Bobo::CreateApplication()
{
	return new Game();
};