#pragma once

namespace Bobo2
{
	using Entity = unsigned int;

	class EntityManager
	{
	public:
		Entity CreateEntity()
		{
			return nextEntityId++;
		}

	private:
		Entity nextEntityId = 0;
	};
}