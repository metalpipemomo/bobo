#pragma once

#include "../../engine/EntityComponent/Component.h"
#include "../../engine/EntityComponent/BaseComponents/Transform.h"
#include <string>

class GameManager : public Component
{
public:
	GameManager() 
	{ 
		stripesAmount = 7;
		solidsAmount = 7;
	}

	int stripesAmount;
	int solidsAmount;

};