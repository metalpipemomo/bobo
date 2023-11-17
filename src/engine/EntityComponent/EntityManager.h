#pragma once

using Entity = unsigned int;

class EntityManager
{
public:
	static Entity CreateEntity()
	{
		static Entity s_NextEntity = 0;
		return s_NextEntity++;
	}
};