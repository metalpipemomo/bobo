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
	}

	~Game()
	{

	}
};

Bobo::Application* Bobo::CreateApplication()
{
	return new Game();
};