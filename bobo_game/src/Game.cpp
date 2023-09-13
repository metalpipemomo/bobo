#include <Bobo.h>
#include "../../bobo_engine/src/Bobo/ModelLoading/ModelLoader.h"

class Game : public Bobo::Application
{
public:
	Game()
	{
		Bobo::ModelLoader::GetInstance().LoadAllModels("src/Models");
	}

	~Game()
	{

	}
};

Bobo::Application* Bobo::CreateApplication()
{
	return new Game();
};