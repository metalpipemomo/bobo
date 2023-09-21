#pragma once

#include "bobopch.h"

namespace Bobo
{
	class System
	{
	public:
		System(std::string inScene);

		virtual ~System() = 0;

		virtual void Update() {}

		virtual void FixedUpdate() {}
	protected:
		std::string m_InScene;
	};
}