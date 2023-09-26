#include <Bobo.h>

using namespace Bobo;

class Game : public Application
{
public:
	Game()
	{
		// Load all Models
		ModelLoader::GetInstance().LoadAllModels("src/Models");

		// Testing Audio
		SoundInfo thwap = SoundInfo("src/TestSFX/Punch.mp3", false, false);
		AudioEngine::GetInstance().LoadSound(thwap);
		AudioEngine::GetInstance().PlaySound(thwap);

		SoundInfo boom = SoundInfo("src/TestSFX/Boom.mp3", false, false);
		AudioEngine::GetInstance().LoadSound(boom);
		AudioEngine::GetInstance().PlaySound(boom);

		// Testing Scenes & ECS
		// Register Scene 1
		Scene* s0 = new Scene("Scene0");
		SceneManager::GetInstance().RegisterScene(s0);

		// Register Scene 2
		Scene* s1 = new Scene("Scene1");
		SceneManager::GetInstance().RegisterScene(s1);
		
		// Load the Scene at SceneIndex 0
		SceneManager::GetInstance().LoadScene(0);

		// Add Entity to Scene 1
		auto entity = s0->CreateEntity();

		// Add Transform Component to Entity
		s0->AddComponent<Transform>(entity, glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1));

		// Create a Child Entity
		auto child = s0->CreateEntity(entity);
		
		// Destroy the child Entity
		s0->DestroyEntity(child);


	}

	~Game()
	{

	}
};

Bobo::Application* Bobo::CreateApplication()
{
	return new Game();
};