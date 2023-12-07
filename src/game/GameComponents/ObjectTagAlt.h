#pragma once

#include "../../engine/EntityComponent/Component.h"
#include "../../engine/EntityComponent/BaseComponents/Transform.h"
#include <string>

class ObjectTagAlt : public Component
{
public:
	ObjectTagAlt(std::string tagg) 
	{ 
		tag = tagg; 
	}

	std::string tag;
};