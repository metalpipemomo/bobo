#include <Bobo.h>

#include <glm/glm.hpp>

class Game : public Bobo::Application
{
public:
	Game()
	{
		glm::vec4 some = glm::vec4(0, 0, 0, 1);
	}

	~Game()
	{

	}
};

Bobo::Application* Bobo::CreateApplication()
{
	return new Game();
};