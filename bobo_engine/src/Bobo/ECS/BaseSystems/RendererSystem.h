#pragma once

#include "bobopch.h"

#include "Bobo/ECS/System.h"
#include "Bobo/ECS/BaseComponents/Transform.h"

namespace Bobo
{
	class RendererSystem : public System
	{
		virtual ~RendererSystem();

		void Update() override;

		void FixedUpdate() override;

	public:
		RendererSystem(std::string inScene) : System(inScene) {};
	};
	
}