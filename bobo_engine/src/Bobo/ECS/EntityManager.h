#pragma once

namespace Bobo
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