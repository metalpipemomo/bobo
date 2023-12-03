#pragma once

#include "../../engine/EntityComponent/Component.h"
#include "../../engine/EntityComponent/BaseComponents/Transform.h"
#include <string>

class ObjectTag : public Component
{
public:
	ObjectTag(std::string tagg) 
	{ 
		tag = tagg; 
	}

	std::string tag;

};