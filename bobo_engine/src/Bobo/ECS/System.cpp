#include "bobopch.h"

#include "System.h"

namespace Bobo
{
	System::~System()
	{
		// 
	}

	System::System(std::string inScene)
	{
		m_InScene = inScene;
	}
}