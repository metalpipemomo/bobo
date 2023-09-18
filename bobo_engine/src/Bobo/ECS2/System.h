#pragma once

namespace Bobo2
{
	class System
	{
	public:
		virtual ~System() = 0;

		virtual void Update() {}

		virtual void FixedUpdate() {}
	};
}