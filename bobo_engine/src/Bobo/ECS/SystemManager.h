#pragma once

#include "System.h"
#include "Scene.h"
#include "BaseSystems/RendererSystem.h"

namespace Bobo
{
	class SystemManager
	{
	public: 
		SystemManager() {}

		void UpdateAll()
		{
			for (System* sys : Scene.GetSystems())
			{
				sys->Update();
			}

			RendererSystem* rendererSystem = Scene.GetSystem(RendererSystem*);

			// We want the renderer system to draw all required things at the very end of the update.
			// rendererSystem.draw()

		}

		void FixedUpdateAll()
		{
			for (System* sys : Scene.GetSystems())
			{
				sys->FixedUpdate();
			}

			RendererSystem* rendererSystem = Scene.GetSystem(RendererSystem*);

			// We want the renderer system to draw all required things at the very end of the update.
			// rendererSystem.draw()

		}

	};
}