#include <Bobo.h>

class Game : public Bobo::Application
{
public:
	Game()
	{

	}

	~Game()
	{

	}
};

Bobo::Application* Bobo::CreateApplication()
{
	return new Game();
};