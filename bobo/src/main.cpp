#include "bpch.h"
#include "engine/Bobo.h"
#include "game/Game.h"

int main()
{
	Window* window = new Window();

	Game* game = new Game();

	window->EventLoop();

	return 0;
}