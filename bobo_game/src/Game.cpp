#include <Bobo.h>

class Game : public Bobo::Application
{
public:
	Game()
	{
		Bobo::ModelLoader::GetInstance().LoadAllModels("src/Models");
		Log("Bruh");
	}

	~Game()
	{

	}
};

Bobo::Application* Bobo::CreateApplication()
{
	return new Game();
};