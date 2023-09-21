#include "bobopch.h"

#include "RendererSystem.h"

namespace Bobo
{
	RendererSystem::RendererSystem()
	{
		p_ActiveScene = nullptr;
	}

	RendererSystem::~RendererSystem()
	{

	}

	void RendererSystem::Update()
	{
		// BOBO_INFO("RendererSystem Update");
	}

	void RendererSystem::FixedUpdate()
	{
		// BOBO_INFO("RendererSystem FixedUpdate");
	}

	void RendererSystem::SetActiveScene(Scene* scene)
	{
		p_ActiveScene = scene;
	}
}