#pragma once

namespace Bobo
{
	class System
	{
	public:
		virtual ~System() = 0;
		virtual void Update() {}
		virtual void FixedUpdate() {}
	};
}